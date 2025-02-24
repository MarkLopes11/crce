


import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.table.DefaultTableModel;

public class SelectOrder {
    private JFrame frame;
    private JComboBox<String> foodMenu;
    private JTextField quantityField;
    private JButton addToOrderButton;
    private JButton placeOrderButton;
    private JLabel priceLabel;  // Added label to display the price
    private JTable orderTable;
    private DefaultTableModel orderModel;

    public SelectOrder() {
        frame = new JFrame("Food Order App");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(500, 300);
        frame.setLayout(new FlowLayout());

        String[] menuItems = {"Burger", "Pizza", "Biryani", "Ice Cream", "Vada Pav", "Samosa"};
        foodMenu = new JComboBox<>(menuItems);
        quantityField = new JTextField(10);
        addToOrderButton = new JButton("Add to Order");
        placeOrderButton = new JButton("Place Order");
        priceLabel = new JLabel("");  // Initialize the label
        orderModel = new DefaultTableModel();
        orderModel.addColumn("Food");
        orderModel.addColumn("Quantity");
        orderModel.addColumn("Price (₹)");  // Modified label to indicate INR
        orderModel.addColumn("Total Price (₹)");  // Modified label to indicate INR
        orderTable = new JTable(orderModel);

        frame.add(new JLabel("Select Food:"));
        frame.add(foodMenu);
        frame.add(new JLabel("Quantity:"));
        frame.add(quantityField);
        frame.add(addToOrderButton);
        frame.add(placeOrderButton);
        frame.add(priceLabel);  // Add the label to the frame
        frame.add(new JScrollPane(orderTable));

        foodMenu.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                String selectedFood = foodMenu.getSelectedItem().toString();
                double price = calculatePrice(selectedFood);
                priceLabel.setText("Price of one: ₹" + price);  // Display price in INR
            }
        });

        addToOrderButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                String selectedFood = foodMenu.getSelectedItem().toString();
                int quantity = Integer.parseInt(quantityField.getText());
                double price = calculatePrice(selectedFood);
                double totalPrice = price * quantity;
                orderModel.addRow(new Object[]{selectedFood, quantity, price, totalPrice});
            }
        });

        placeOrderButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                displayOrderSummary();
            }
        });

        frame.setVisible(true);
    }

    private double calculatePrice(String food) {
        
        double price = 0.0;
        if (food.equals("Burger")) {
            price = 80.0;  // Price in INR
        } else if (food.equals("Pizza")) {
            price = 200.0;  // Price in INR
        } else if (food.equals("Biryani")) {
            price = 150.0;  // Price in INR
        } else if (food.equals("Ice Cream")) {
            price = 30.0;  // Price in INR
        } else if (food.equals("Vada Pav")) {
            price = 10.0;  // Price in INR
        } else if (food.equals("Samosa")) {
            price = 15.0;  // Price in INR
        }
        return price;
    }

    private void displayOrderSummary() {
        JFrame orderSummaryFrame = new JFrame("Order Summary");
        orderSummaryFrame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        orderSummaryFrame.setSize(400, 300);

        JPanel summaryPanel = new JPanel(new BorderLayout());
        summaryPanel.add(new JScrollPane(orderTable), BorderLayout.CENTER);

        double totalOrderPrice = calculateTotalOrderPrice();
        summaryPanel.add(new JLabel("Total Price: ₹" + totalOrderPrice), BorderLayout.SOUTH);  // Display total price in INR

        orderSummaryFrame.add(summaryPanel);
        orderSummaryFrame.setVisible(true);
    }

    private double calculateTotalOrderPrice() {
        double total = 0.0;
        for (int row = 0; row < orderModel.getRowCount(); row++) {
            double totalPrice = (double) orderModel.getValueAt(row, 3);
            total += totalPrice;
        }
        return total;
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new SelectOrder();
            }
        });
    }
}