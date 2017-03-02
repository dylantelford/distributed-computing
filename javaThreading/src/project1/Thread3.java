package project1;

/**
 * Thread3 class will read through its portion of the 2D users array, starting at 
 * its start value and finishing at its stop value. It will count the number of 
 * users who viewed a certain category X more than a category Y. Both 
 * of which are passed into the constructor. It is used by query 5. 
 * 
 * @author Dylan Telford
 *
 */
public class Thread3 extends Thread {
	
	private int start;
	private int stop;
	private int pageX;
	private int pageY;
	private int total;
	private int[][] myArray;

	/**
	 * constructor for the Thread3 class will initialize the start, stop, pageX, pageY, 
	 * and total variables. 
	 * 
	 * @param start index of the array to start reading at
	 * @param stop index of the array to read through 
	 * @param selectionX category selection for X
	 * @param selectionY category selection for Y
	 */
	public Thread3(int start, int stop, String selectionX, String selectionY) {
		myArray = MyMain.users;
		this.start = start;
		this.stop = stop;
		pageX = MyMain.pages.get(selectionX);
		pageY = MyMain.pages.get(selectionY);
		total = 0;
	}
	
	/**
	 * FOR TESTING PURPOSES ONLY
	 * uses testArray from myMain class. only called by JUnit Test. 
	 * 
	 * @param test
	 * @param start
	 * @param stop
	 * @param page
	 * @param num
	 */
	public Thread3(String test, int start, int stop, int pageX, int pageY) {
		myArray = MyMain.testArray;
		this.start = start;
		this.stop = stop;
		this.pageX = pageX;
		this.pageY = pageY;
		total = 0;
	}
	
	/**
	 * run method will read through the object's section of the users 2D array. It will
	 * check to see if the index of pageX is higher than the index of pageY - if it is, total
	 * will be incremented. It will yield to the other threads at every fifth of its section. 
	 */
	public void run() {
		int yieldTime = (start-stop)/QueryHandler.numThreads;
		for (int i=start; i<=stop; i++) {
			if (myArray[i][pageX] > myArray[i][pageY]) {
				total += 1;
			}
			if (i%yieldTime == 0) {
				yield();
			}
		}
	}
	
	/**
	 * getTotal method will return the total number of users counted by the trhead who 
	 * viewed X more than Y. 
	 * 
	 * @return total
	 */
	public int getTotal() {
		return total;
	}
}
