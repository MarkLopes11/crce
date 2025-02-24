import javax.swing.*;
import java.awt.*;

public class MyFrame extends JFrame {

    
	public MyFrame() 
	{
        setTitle("JSeparator");

        JPanel panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));

        JLabel label1 = new JLabel("Label 1");
        JLabel label2 = new JLabel("Label 2");

        JSeparator separator = new JSeparator(JSeparator.HORIZONTAL);

        JLabel label3 = new JLabel("Label 3");
        JLabel label4 = new JLabel("Label 4");

        panel.add(label1);
        panel.add(label2);
        panel.add(separator);
        panel.add(label3);
        panel.add(label4);

        add(panel);
    }

    public static void main(String[] args) 
    	{
        	MyFrame f = new MyFrame();
        	f.setSize(300, 200);
        	f.setVisible(true);
        
        	f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        }
    }
