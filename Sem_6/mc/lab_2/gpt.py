#!/usr/bin/python

from math import *
from tkinter import *

# Base class for Hexagon shape
class Hexagon(object):
    def __init__(self, parent, x, y, length, color, tags):
        self.parent = parent
        self.x = x
        self.y = y
        self.length = length
        self.color = color
        self.tags = tags
        self.draw_hex()

    # draw one hexagon
    def draw_hex(self):
        start_x = self.x
        start_y = self.y
        angle = 60
        coords = []
        for i in range(6):
            end_x = start_x + self.length * cos(radians(angle * i))
            end_y = start_y + self.length * sin(radians(angle * i))
            coords.append((start_x, start_y))  # Tuple for polygon coordinates
            start_x, start_y = end_x, end_y  # Update start_x and start_y
        self.parent.create_polygon(coords, fill=self.color, outline="black", tags=self.tags)

# class holds frequency reuse logic and related methods
class FrequencyReuse(Tk):
    CANVAS_WIDTH = 800
    CANVAS_HEIGHT = 650
    TOP_LEFT = (20, 20)
    BOTTOM_LEFT = (790, 560)  # Not used, consider removing
    TOP_RIGHT = (780, 20)
    BOTTOM_RIGHT = (780, 560)

    def __init__(self, cluster_size, columns=16, rows=10, edge_len=30):
        Tk.__init__(self)
        self.textbox = None
        self.curr_angle = 330
        self.first_click = True
        self.reset = False  # Not used, consider removing
        self.edge_len = edge_len
        self.cluster_size = cluster_size
        self.reuse_list = []
        self.all_selected = False  # Not used, consider removing
        self.curr_count = 0
        self.hexagons = []
        self.co_cell_endp = []
        self.reuse_xy = []  # Not used, consider removing
        self.canvas = Canvas(self,
                            width=self.CANVAS_WIDTH,
                            height=self.CANVAS_HEIGHT,
                            bg="#4dd0e1")
        self.canvas.bind("<Button-1>", self.call_back)
        self.canvas.focus_set()
        self.canvas.bind('<Shift-R>', self.resets)
        self.canvas.pack()
        self.title("Frequency reuse and co-channel selection")
        self.create_grid(columns, rows)  # Pass columns and rows
        self.create_textbox()
        self.cluster_reuse_calc()
        self.i = None  # Store i and j for reuse calculations
        self.j = None


    # show lines joining all co-channel cells
    def show_lines(self):
        # center(x,y) of first hexagon
        if not self.co_cell_endp:
            return  # No lines to draw if co_cell_endp is empty

        approx_center = self.co_cell_endp[0]
        self.line_ids = []
        for k in range(1, len(self.co_cell_endp)):
            end_xx = self.co_cell_endp[k][0]
            end_yy = self.co_cell_endp[k][1]

            # Draw a line between the first selected cell and each calculated co-channel cell
            l_id = self.canvas.create_line(approx_center[0], approx_center[1],
                                        end_xx, end_yy, fill="blue")  # Added fill color
            self.line_ids.append(l_id)



    def create_textbox(self):
        txt = Text(self.canvas,
                width=80,
                height=1,
                font=("Helvatica", 12),
                padx=10,
                pady=10)
        txt.tag_configure("center", justify="center")
        txt.insert("1.0", "Select a Hexagon")
        txt.tag_add("center", "1.0", "end")
        self.canvas.create_window((self.CANVAS_WIDTH // 2, 600),  window=txt) # Centered
        txt.config(state=DISABLED)
        self.textbox = txt

    def resets(self, event):
        if event.char == 'R':
            self.reset_grid()

    # clear hexagonal grid for new i/p
    def reset_grid(self, button_reset=False):
        self.first_click = True
        self.curr_angle = 330
        self.curr_count = 0
        self.co_cell_endp = []
        self.reuse_list = []

        for hexagon in self.hexagons:
            self.canvas.itemconfigure(hexagon.tags, fill=hexagon.color)

        if hasattr(self, 'line_ids'):  # Check if line_ids exists
            for line_id in self.line_ids:
                self.canvas.delete(line_id)
            self.line_ids = []

        if button_reset:
            self.write_text("Select a Hexagon")

    # create a grid of Hexagons
    def create_grid(self, cols, rows):
        size = self.edge_len
        for c in range(cols):
            offset = size * sqrt(3) / 2 if c % 2 else 0
            for r in range(rows):
                x = c * (self.edge_len * 1.5) + 50
                y = (r * (self.edge_len * sqrt(3))) + offset + 15
                hx = Hexagon(self.canvas, x, y, self.edge_len, "#fafafa",
                            "{},{}".format(r, c))
                self.hexagons.append(hx)

    # calculate reuse distance, center distance and radius of the hexagon
    def cluster_reuse_calc(self):
        self.hex_radius = sqrt(3) / 2 * self.edge_len
        self.center_dist = sqrt(3) * self.hex_radius
        self.reuse_dist = self.hex_radius * sqrt(3 * self.cluster_size) # Not directly used in the core logic

    def write_text(self, text):
        self.textbox.config(state=NORMAL)
        self.textbox.delete('1.0', END)
        self.textbox.insert('1.0', text, "center")
        self.textbox.config(state=DISABLED)

    #check if the co-channels are within visible canvas
    def is_within_bound(self, coords):
        return self.TOP_LEFT[0] < coords[0] < self.BOTTOM_RIGHT[0] \
               and self.TOP_RIGHT[1] < coords[1] < self.BOTTOM_RIGHT[1]

    #gets called when user selects a hexagon
    def call_back(self, evt):
        selected_hex_id = self.canvas.find_closest(evt.x, evt.y)[0]
        hexagon = self.hexagons[int(selected_hex_id - 1)]
        s_x, s_y = hexagon.x, hexagon.y
        approx_center = (s_x + 15, s_y + 25)  # Approximate center for line drawing

        if self.first_click:
            self.first_click = False
            self.write_text(
                """Now, select another hexagon such 
                   that it should be a co-cell of
                   the original hexagon."""
            )
            self.co_cell_endp.append(approx_center)
            self.canvas.itemconfigure(hexagon.tags, fill="green")

            # Calculate co-channel cell positions based on i and j
            temp_co_cell_endp = [approx_center]
            initial_angle = 330 # Reset initial angle for each first click
            for _ in range(6):
                end_xx = approx_center[0] + self.center_dist * self.i * cos(radians(initial_angle))
                end_yy = approx_center[1] + self.center_dist * self.i * sin(radians(initial_angle))

                reuse_x = end_xx + (self.center_dist * self.j) * cos(radians(initial_angle - 60))
                reuse_y = end_yy + (self.center_dist * self.j) * sin(radians(initial_angle - 60))

                if self.is_within_bound((reuse_x, reuse_y)):
                    temp_co_cell_endp.append((reuse_x, reuse_y))

                    # Find the hexagon closest to the calculated coordinates and add its ID to reuse_list
                    closest_hex_id = self.canvas.find_closest(reuse_x, reuse_y)[0]
                    self.reuse_list.append(closest_hex_id)

                initial_angle -= 60

            #Filter out the initial selection, only keep the reuse cells:
            self.co_cell_endp = temp_co_cell_endp[1:]
            # Ensure the reuse list does *not* include the initial clicked hexagon.
            if selected_hex_id in self.reuse_list:
              self.reuse_list.remove(selected_hex_id)
            

        else:
            curr = self.canvas.find_closest(s_x, s_y)[0]
            if curr in self.reuse_list:
                self.canvas.itemconfigure(hexagon.tags, fill="green")
                self.write_text("Correct! Cell {} is a co-cell.".format(hexagon.tags))
                self.curr_count += 1

                if self.curr_count == len(self.reuse_list):
                    self.write_text("Great! Press Shift-R to restart")
                    self.show_lines()
            else:
                self.write_text("Incorrect! Cell {} is not a co-cell.".format(hexagon.tags))
                self.canvas.itemconfigure(hexagon.tags, fill="red")


if __name__ == '__main__':
    print(
        """Enter i & j values. common (i,j) values are: 
           (1,0), (1,1), (2,0), (2,1), (3,0), (2,2)"""
    )
    i = int(input("Enter i: "))
    j = int(input("Enter j: "))
    if i == 0 and j == 0:
        raise ValueError("i & j both cannot be zero")
    elif j > i and (i,j) != (1,1) :
      #Allow (1,1)
        raise ValueError("value of j cannot be greater than i")

    else:
        N = (i**2 + i * j + j**2)
        print("N is {}".format(N))
    freqreuse = FrequencyReuse(cluster_size=N)
    freqreuse.i = i  # Store i
    freqreuse.j = j  # Store j
    freqreuse.mainloop()