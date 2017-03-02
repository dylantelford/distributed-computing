package project1;

/**
 * Thread1 class will read through its portion of the 2D users array, starting at 
 * its start value and finishing at its stop value. It will count the number of 
 * users who viewed a certain category passed into the constructor. 
 * Used by queries 1, 2, and 3. 
 * 
 * @author Dylan Telford
 *
 */
public class Thread1 extends Thread {
	
	private int start;
	private int stop;
	private int page;
	private int total;
	private int[][] myArray;
	
	/**
	 * constructor initialized start, stop, page, and total variables. 
	 * 
	 * @param start index to start reading array at
	 * @param stop index to stop reading array at
	 * @param selection category selection 
	 */
	public Thread1(int start, int stop, String selection) {
		myArray = MyMain.users;
		this.start = start;
		this.stop = stop;
		page = MyMain.pages.get(selection);
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
	 */
	public Thread1(String test, int start, int stop, int page) {
		myArray = MyMain.testArray;
		this.start = start;
		this.stop = stop;
		this.page = page;
		total = 0;
	}
	
	/**
	 * run method traverses section of the array, increments total when a user has viewed 
	 * the page, and yields to the other threads at every fifth of the way through its section. 
	 */
	public void run() {
		int yieldTime = (start-stop)/QueryHandler.numThreads;
		for (int i=start; i<=stop; i++) {
			if (myArray[i][page] > 0) {
				total += 1;
			}
			if (i%yieldTime == 0) {
				yield();
			}
		}
	}
	
	/**
	 * returns value of total
	 * @return total 
	 */
	public int getTotal() {
		return total;
	}
}
