package project1;

import javax.swing.JFrame;
import javax.swing.JPanel;
import java.awt.CardLayout;
import javax.swing.JRadioButton;
import javax.swing.JLabel;
import java.awt.Font;
import javax.swing.JOptionPane;
import javax.swing.JTextField;
import javax.swing.JComboBox;
import javax.swing.JButton;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

/**
 * MyGUI class makes the user interface to control the flow of the application. 
 * The user will select a query, and will choose categories and/or enter numbers 
 * to run the query. Each time the user runs a query, an object of QueryHandler 
 * is made to create the necessary threads and compute the answer. 
 * A JOptionPane window will come up displaying the answer to the user. 
 * 
 * @author Dylan Telford
 * made using Java Window Builder
 */
public class MyGUI {
	private String[] categories = {"Frontpage", "News", "Tech", "Local", "Opinion", "On-Air", "Miscelanious", 
			"Weather", "MSN-News", "Health", "Living", "Business", "MSN-Sports", "Sports", "Summary", 
			"BBS", "Travel"};
	JFrame frame;
	private JPanel panelMenu;
	private JPanel panelQ1;
	private JPanel panelQ2;
	private JPanel panelQ3;
	private JPanel panelQ4;
	private JPanel panelQ5;
	private JRadioButton rdbtnQry1;
	private JRadioButton rdbtnQry2;
	private JRadioButton rdbtnQry3;
	private JRadioButton rdbtnQry4;
	private JRadioButton rdbtnQry5;
	private JButton btnQ1;
	private JButton btnQ2;
	private JButton btnQ3;
	private JButton btnQ4;
	private JButton btnQ5;
	private JLabel lblPickQry;
	private JLabel lblQ1;
	private JLabel lblQ1p1;
	private JLabel lblQ1p2;
	private JLabel lblQ2;
	private JLabel lblQ2p1;
	private JLabel lblQ3;
	private JLabel lblQ3p1;
	private JLabel lblQ3p2;
	private JLabel lblQ4;
	private JLabel lblQ4p1;
	private JLabel lblQ4p2;
	private JLabel lblQ5;
	private JLabel lblQ5p1;
	private JLabel lblQ5p2;
	private JTextField txtQ1userNum;
	private JTextField txtQ4userNum;
	private JComboBox<String> comboBoxQ1x;
	private JComboBox<String> comboBoxQ2x;
	private JComboBox<String> comboBoxQ3x;
	private JComboBox<String> comboBoxQ3y;
	private JComboBox<String> comboBoxQ4x;
	private JComboBox<String> comboBoxQ5x;
	private JComboBox<String> comboBoxQ5y;

	/**
	 * constructor creates the application.
	 */
	public MyGUI() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame. Uses a card layout with a main menu and 
	 * 5 menus for each of the queries. 
	 * 
	 */
	private void initialize() {
		//main JFrame Object with card layout
		frame = new JFrame();
		frame.setBounds(100, 100, 450, 300);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(new CardLayout(0, 0));
		
		//JPanel for main menu, will contain 5 JRadioButtons which make their query panel visible when checked
		panelMenu = new JPanel();
		frame.getContentPane().add(panelMenu, "name_1443450867905480000");
		panelMenu.setLayout(null);
		
		lblPickQry = new JLabel("Pick a query below:");
		lblPickQry.setFont(new Font("Lucida Grande", Font.BOLD, 15));
		lblPickQry.setBounds(63, 34, 187, 16);
		panelMenu.add(lblPickQry);
		
		rdbtnQry1 = new JRadioButton("Are there more than ___ users who looked at X?");
		rdbtnQry1.setBounds(63, 62, 349, 23);
		panelMenu.add(rdbtnQry1);
		rdbtnQry1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				panelQ1.setVisible(true);
				panelMenu.setVisible(false);
				rdbtnQry1.setSelected(false);
			}
		});
		
		rdbtnQry2 = new JRadioButton("What percent of users looked at X?");
		rdbtnQry2.setBounds(63, 95, 349, 23);
		panelMenu.add(rdbtnQry2);
		rdbtnQry2.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				panelQ2.setVisible(true);
				panelMenu.setVisible(false);
				rdbtnQry2.setSelected(false);
			}
		});

		rdbtnQry3 = new JRadioButton("Are there more users who looked at X than Y?");
		rdbtnQry3.setBounds(63, 130, 356, 23);
		panelMenu.add(rdbtnQry3);
		rdbtnQry3.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				panelQ3.setVisible(true);
				panelMenu.setVisible(false);
				rdbtnQry3.setSelected(false);
			}
		});
		
		rdbtnQry4 = new JRadioButton("How many users viewed X ___ number of times?");
		rdbtnQry4.setBounds(63, 165, 349, 23);
		panelMenu.add(rdbtnQry4);
		rdbtnQry4.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				panelQ4.setVisible(true);
				panelMenu.setVisible(false);
				rdbtnQry4.setSelected(false);
			}
		});
		
		rdbtnQry5 = new JRadioButton("What percent of users looked at X more than Y?");
		rdbtnQry5.setBounds(63, 200, 349, 23);
		panelMenu.add(rdbtnQry5);
		rdbtnQry5.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				panelQ5.setVisible(true);
				panelMenu.setVisible(false);
				rdbtnQry5.setSelected(false);
			}
		});
		
		//JPanel for Query 1, contains labels for question, JTextField for user number, JComboBox for page select
		//Each Query Panel has a search button that calls method in QueryHandler with selections as arguments
		panelQ1 = new JPanel();
		frame.getContentPane().add(panelQ1, "name_1443450681221714000");
		panelQ1.setLayout(null);
		
		lblQ1 = new JLabel("Query 1:");
		lblQ1.setFont(new Font("Lucida Grande", Font.BOLD, 15));
		lblQ1.setBounds(16, 44, 95, 16);
		panelQ1.add(lblQ1);
		
		lblQ1p1 = new JLabel("Are there more than");
		lblQ1p1.setBounds(16, 101, 133, 16);
		panelQ1.add(lblQ1p1);
		
		txtQ1userNum = new JTextField();
		txtQ1userNum.setBounds(147, 101, 42, 16);
		panelQ1.add(txtQ1userNum);
		txtQ1userNum.setColumns(10);
		
		lblQ1p2 = new JLabel("users who looked at ");
		lblQ1p2.setBounds(193, 101, 133, 16);
		panelQ1.add(lblQ1p2);
		
		comboBoxQ1x = new JComboBox<String>(categories);
		comboBoxQ1x.setBounds(324, 97, 106, 27);
		panelQ1.add(comboBoxQ1x);
		
		btnQ1 = new JButton("Search");
		btnQ1.setBounds(16, 160, 117, 29);
		panelQ1.add(btnQ1);
		btnQ1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				//call query method
				String sel = comboBoxQ1x.getSelectedItem().toString();
				int num = Integer.parseInt(txtQ1userNum.getText());
				QueryHandler q = new QueryHandler(1, sel, num);
				String message = q.getAnswer();
				JOptionPane.showMessageDialog(null, message);
				
				panelMenu.setVisible(true);
				panelQ1.setVisible(false);
			}
		});
		
		//JPanel for Query 2, contains label for question and JComboBox for page select
		panelQ2 = new JPanel();
		frame.getContentPane().add(panelQ2, "name_1443450691363722000");
		panelQ2.setLayout(null);
		
		lblQ2 = new JLabel("Query 2:");
		lblQ2.setFont(new Font("Lucida Grande", Font.BOLD, 15));
		lblQ2.setBounds(16, 50, 154, 16);
		panelQ2.add(lblQ2);
		
		lblQ2p1 = new JLabel("What percent of users looked at ");
		lblQ2p1.setBounds(16, 101, 204, 16);
		panelQ2.add(lblQ2p1);
		
		comboBoxQ2x = new JComboBox<String>(categories);
		comboBoxQ2x.setBounds(220, 97, 108, 27);
		panelQ2.add(comboBoxQ2x);
		
		btnQ2 = new JButton("Search");
		btnQ2.setBounds(16, 150, 117, 29);
		panelQ2.add(btnQ2);
		btnQ2.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				//call query method
				String sel = comboBoxQ2x.getSelectedItem().toString();
				QueryHandler q = new QueryHandler(2, sel);
				String message = q.getAnswer();
				JOptionPane.showMessageDialog(null,  message);
				
				panelMenu.setVisible(true);
				panelQ2.setVisible(false);
			}
		});
		
		//JPanel for Query 3, contains labels, JComboBox's for x and y page selections
		panelQ3 = new JPanel();
		frame.getContentPane().add(panelQ3, "name_1443450696608070000");
		panelQ3.setLayout(null);
		
		lblQ3 = new JLabel("Query 3:");
		lblQ3.setFont(new Font("Lucida Grande", Font.BOLD, 15));
		lblQ3.setBounds(16, 50, 83, 16);
		panelQ3.add(lblQ3);
		
		lblQ3p1 = new JLabel("Are there more users who looked at ");
		lblQ3p1.setBounds(16, 100, 228, 16);
		panelQ3.add(lblQ3p1);
		
		comboBoxQ3x = new JComboBox<String>(categories);
		comboBoxQ3x.setBounds(244, 96, 89, 27);
		panelQ3.add(comboBoxQ3x);
		
		lblQ3p2 = new JLabel("than");
		lblQ3p2.setBounds(333, 100, 36, 16);
		panelQ3.add(lblQ3p2);
		
		comboBoxQ3y = new JComboBox<String>(categories);
		comboBoxQ3y.setBounds(360, 96, 84, 27);
		panelQ3.add(comboBoxQ3y);
		
		btnQ3 = new JButton("Search");
		btnQ3.setBounds(16, 150, 117, 29);
		panelQ3.add(btnQ3);
		btnQ3.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				//call query method
				String sel1 = comboBoxQ3x.getSelectedItem().toString();
				String sel2 = comboBoxQ3y.getSelectedItem().toString();
				QueryHandler q = new QueryHandler(3, sel1, sel2);
				String message = q.getAnswer();
				JOptionPane.showMessageDialog(null,  message);
				
				panelMenu.setVisible(true);
				panelQ3.setVisible(false);
			}
		});
		
		//JPanel for Query 4, contains labels, JComboBox for page selection and JTextField for user number
		panelQ4 = new JPanel();
		frame.getContentPane().add(panelQ4, "name_1443450699180023000");
		panelQ4.setLayout(null);
		
		lblQ4 = new JLabel("Query 4:");
		lblQ4.setFont(new Font("Lucida Grande", Font.BOLD, 15));
		lblQ4.setBounds(16, 50, 96, 16);
		panelQ4.add(lblQ4);
		
		lblQ4p1 = new JLabel("How many users viewed");
		lblQ4p1.setBounds(16, 100, 157, 16);
		panelQ4.add(lblQ4p1);
		
		comboBoxQ4x = new JComboBox<String>(categories);
		comboBoxQ4x.setBounds(169, 96, 96, 27);
		panelQ4.add(comboBoxQ4x);
		
		txtQ4userNum = new JTextField();
		txtQ4userNum.setBounds(262, 94, 56, 28);
		panelQ4.add(txtQ4userNum);
		txtQ4userNum.setColumns(10);
		
		lblQ4p2 = new JLabel("number of times");
		lblQ4p2.setBounds(319, 100, 104, 16);
		panelQ4.add(lblQ4p2);
		
		btnQ4 = new JButton("Search");
		btnQ4.setBounds(16, 150, 117, 29);
		panelQ4.add(btnQ4);
		btnQ4.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				//call query method
				String sel = comboBoxQ4x.getSelectedItem().toString();
				int num = Integer.parseInt(txtQ4userNum.getText());
				QueryHandler q = new QueryHandler(4, sel, num);
				String message = q.getAnswer();
				JOptionPane.showMessageDialog(null, message);
				
				panelMenu.setVisible(true);
				panelQ4.setVisible(false);
			}
		});
		
		//JPanel for Query 5, contains labels, JComboBoxes for page selects x and y
		panelQ5 = new JPanel();
		frame.getContentPane().add(panelQ5, "name_1443450702067787000");
		panelQ5.setLayout(null);
		
		lblQ5 = new JLabel("Query 5:");
		lblQ5.setFont(new Font("Lucida Grande", Font.BOLD, 15));
		lblQ5.setBounds(16, 50, 89, 16);
		panelQ5.add(lblQ5);
		
		lblQ5p1 = new JLabel("What percent of users viewed");
		lblQ5p1.setBounds(16, 100, 190, 16);
		panelQ5.add(lblQ5p1);
		
		comboBoxQ5x = new JComboBox<String>(categories);
		comboBoxQ5x.setBounds(204, 96, 82, 27);
		panelQ5.add(comboBoxQ5x);
		
		lblQ5p2 = new JLabel("more than");
		lblQ5p2.setBounds(287, 100, 70, 16);
		panelQ5.add(lblQ5p2);
		
		comboBoxQ5y = new JComboBox<String>(categories);
		comboBoxQ5y.setBounds(355, 96, 82, 27);
		panelQ5.add(comboBoxQ5y);
		
		btnQ5 = new JButton("Search");
		btnQ5.setBounds(16, 150, 117, 29);
		panelQ5.add(btnQ5);
		btnQ5.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				//call query method
				String sel1 = comboBoxQ5x.getSelectedItem().toString();
				String sel2 = comboBoxQ5y.getSelectedItem().toString();
				QueryHandler q = new QueryHandler(5, sel1, sel2);
				String message = q.getAnswer();
				JOptionPane.showMessageDialog(null, message);
				
				panelMenu.setVisible(true);
				panelQ5.setVisible(false);
			}
		});
	}
}