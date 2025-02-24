// Importing the required packages
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

// Creating a class that extends JFrame and implements ActionListener
public class FoodOrderingSystem extends JFrame implements ActionListener {

    // Declaring the components of the GUI
    private JLabel title, veg, nonveg, total;
    private JCheckBox idli, dosa, chicken, mutton;
    private JTextField idliQty, dosaQty, chickenQty, muttonQty;
    private JButton order, reset;

    // Creating a constructor that sets up the GUI
    public FoodOrderingSystem() {
        // Setting the title, size and layout of the frame
        super("Food Ordering System");
        setSize(400, 300);
        setLayout(new GridLayout(6, 2));

        // Initializing the components
        title = new JLabel("Welcome to Food Ordering System", JLabel.CENTER);
        veg = new JLabel("Veg Items", JLabel.CENTER);
        nonveg = new JLabel("Non-Veg Items", JLabel.CENTER);
        total = new JLabel("Total: Rs. 0", JLabel.CENTER);
        idli = new JCheckBox("Idli: Rs. 25");
        dosa = new JCheckBox("Dosa: Rs. 30");
        chicken = new JCheckBox("Chicken: Rs. 150");
        mutton = new JCheckBox("Mutton: Rs. 430");
        idliQty = new JTextField("0");
        dosaQty = new JTextField("0");
        chickenQty = new JTextField("0");
        muttonQty = new JTextField("0");
        order = new JButton("Order");
        reset = new JButton("Reset");

        // Adding action listeners to the buttons and checkboxes
        order.addActionListener(this);
        reset.addActionListener(this);
        idli.addActionListener(this);
        dosa.addActionListener(this);
        chicken.addActionListener(this);
        mutton.addActionListener(this);

        // Adding the components to the frame
        add(title);
        add(new JLabel()); // Empty label for spacing
        add(veg);
        add(nonveg);
        add(idli);
        add(chicken);
        add(idliQty);
        add(chickenQty);
        add(dosa);
        add(mutton);
        add(dosaQty);
        add(muttonQty);
        add(order);
        add(reset);
        add(total);

        // Setting the visibility of the frame to true
        setVisible(true);

    }

    // Defining the actionPerformed method to handle the events
    public void actionPerformed(ActionEvent e) {

        // Getting the source of the event
        Object source = e.getSource();

        // If order button is clicked, calculate and display the total amount
        if (source == order) {
            int amount = 0; // Variable to store the total amount

            // Checking if idli is selected and adding its price to the amount
            if (idli.isSelected()) {
                int qty = Integer.parseInt(idliQty.getText()); // Getting the quantity of idli
                amount += qty * 25; // Multiplying the quantity by the price and adding to the amount
            }

            // Checking if dosa is selected and adding its price to the amount
            if (dosa.isSelected()) {
                int qty = Integer.parseInt(dosaQty.getText()); // Getting the quantity of dosa
                amount += qty * 30; // Multiplying the quantity by the price and adding to the amount
            }

            // Checking if chicken is selected and adding its price to the amount
            if (chicken.isSelected()) {
                int qty = Integer.parseInt(chickenQty.getText()); // Getting the quantity of chicken
                amount += qty * 150; // Multiplying the quantity by the price and adding to the amount
            }

            // Checking if mutton is selected and adding its price to the amount
            if (mutton.isSelected()) {
                int qty = Integer.parseInt(muttonQty.getText()); // Getting the quantity of mutton
                amount += qty * 430; // Multiplying the quantity by the price and adding to the amount
            }

            // Setting the text of the total label to display the amount
            total.setText("Total: Rs. " + amount);

        }

        // If reset button is clicked, clear all the selections and text fields
        if (source == reset) {
            idli.setSelected(false); // Unchecking idli checkbox
            dosa.setSelected(false); // Unchecking dosa checkbox
            chicken.setSelected(false); // Unchecking chicken checkbox
            mutton.setSelected(false); // Unchecking mutton checkbox

            idliQty.setText("0"); // Setting idli quantity to zero
            dosaQty.setText("0"); // Setting dosa quantity to zero
            chickenQty.setText("0"); // Setting chicken quantity to zero
            muttonQty.setText("0"); // Setting mutton quantity to zero

            total.setText("Total: Rs. 0"); // Setting total amount to zero
        }

        // If any checkbox is clicked, enable or disable the corresponding text field
        if (source == idli) {
            idliQty.setEnabled(idli.isSelected()); // Enabling idli quantity if idli is selected, otherwise disabling it
        }

        if (source == dosa) {
            dosaQty.setEnabled(dosa.isSelected()); // Enabling dosa quantity if dosa is selected, otherwise disabling it
        }

        if (source == chicken) {
            chickenQty.setEnabled(chicken.isSelected()); // Enabling chicken quantity if chicken is selected, otherwise disabling it
        }

        if (source == mutton) {
            muttonQty.setEnabled(mutton.isSelected()); // Enabling mutton quantity if mutton is selected, otherwise disabling it
        }
    }

    // Creating a main method to run the program
    public static void main(String[] args) {
        // Creating an object of the class
        FoodOrderingSystem fos = new FoodOrderingSystem();
    }
}