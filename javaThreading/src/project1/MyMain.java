package project1;

import java.awt.EventQueue;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.TreeMap;

/**
 * MyMain class contains main method and data structures used by other classes. 
 * It will read the data file into an array, set up a TreeMap of the categories,
 * and open the GUI, which will control the flow of the program. 
 * 
 * @author Dylan Telford
 *
 */
public class MyMain {
	/*
	 * users is a 2D array where each user is represented by a row, and each row has 
	 * 17 columns, each representing the number of views of a page by that user.
	 * The number of users is hard coded, making this less flexible to use with other data. 
	 * 
	 * pages is a TreeMap that maps the category names to their index in the users array. 
	 */
	public static int[][] users = new int[989818][17];
	public static TreeMap<String, Integer> pages;
	
	//array used for JUnit Test
	public static int[][] testArray = 
		   {{5, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 1, 7, 3, 0, 0}, 
			{5, 5, 0, 0, 0, 0, 5, 9, 3, 0, 0, 0, 0, 0, 0, 0, 0}, 
			{7, 3, 5, 2, 0, 0, 0, 0, 0, 0, 0, 6, 2, 0, 9, 0, 0}, 
			{5, 0, 2, 6, 0, 0, 0, 0, 0, 0, 4, 6, 3, 0, 0, 0, 0},
			{5, 1, 3, 5, 2, 0, 6, 3, 6, 7, 0, 0, 0, 3, 8, 0, 0}, 
			{5, 2, 0, 0, 0, 0, 0, 4, 7, 1, 0, 0, 0, 0, 0, 0, 0},
			{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{5, 2, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{5, 2, 6, 3, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6},
			{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
	
	/**
	 * main method will make calls to methods to read the data, make the pages TreeMap, and
	 * start the GUI. Everything beyond that point will be controlled by the MyGUI class. 
	 * @param args
	 */
	public static void main(String[] args) {
		readAndStore();
		makeMap();
		openGUI();
	}

	/**
	 * readAndStore method will create a Scanner object to read the data file, 
	 * and will increment the integer at the corresponding row/column in the users
	 * 2D array. 
	 */
	private static void readAndStore() {
		System.out.println("Reading in data file...");
		File inFile = new File("msnbc990928.txt");
		Scanner data = null;
		try {
			data = new Scanner(inFile);
		}
		catch(FileNotFoundException e) {
			System.out.println("Unable to open file msnbc990928.txt");
			e.printStackTrace();
			System.exit(0);
		}
		
		int i = 0;
		while (data.hasNextLine()) {
			String line = data.nextLine();
			String[] numStrings = line.split(" ");
			for (String numString : numStrings) {
				users[i][Integer.parseInt(numString)-1] += 1;
			}
			i += 1;
		}
		data.close();
	}
	
	/**
	 * makeMap method will initialize the pages TreeMap and add each category, mapping
	 * it to the column index that represents it in the users 2D array.
	 */
	private static void makeMap() {
		pages = new TreeMap<String, Integer>();
		//**int values match the index of the matrix, not the numbers in data file...
		pages.put("Frontpage", 0);
		pages.put("News", 1);
		pages.put("Tech", 2);
		pages.put("Local", 3);
		pages.put("Opinion", 4);
		pages.put("On-Air", 5);
		pages.put("Miscelanious", 6);
		pages.put("Weather", 7);
		pages.put("MSN-News", 8);
		pages.put("Health", 9);
		pages.put("Living", 10);
		pages.put("Business", 11);
		pages.put("MSN-Sports", 12);
		pages.put("Sports", 13);
		pages.put("Summary", 14);
		pages.put("BBS", 15);
		pages.put("Travel", 16);
	}
	
	/**
	 * openGUI method simply opens the GUI, allowing the user to interact with the data. 
	 */
	private static void openGUI() {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					MyGUI window = new MyGUI();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
}
