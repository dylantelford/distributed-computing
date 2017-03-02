package project1;

/**
 * Thread2 class will read through its portion of the 2D users array, starting at 
 * its start value and finishing at its stop value. It will count the number of 
 * users who viewed a certain category a certain number of times. Both 
 * of which are passed into the constructor. It is used by query 4. 
 * 
 * @author Dylan Telford
 *
 */
public class Thread2 extends Thread {
	
	private int start;
	private int stop;
	private int num;
	private int page;
	private int total;
	private int[][] myArray;

	/**
	 * constructor for Thread2 will initialze start, stop, num, page, and total variables. 
	 * 
	 * @param start array index to start reading at
	 * @param stop array index to stop reading at
	 * @param num number of users to compare array value to 
	 * @param selection category selection to look for
	 */
	public Thread2(int start, int stop, int num, String selection) {
		myArray = MyMain.users;
		this.start = start;
		this.stop = stop;
		this.num = num;
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
	 * @param num
	 */
	public Thread2(String test, int start, int stop, int page, int num) {
		myArray = MyMain.testArray;
		this.start = start;
		this.stop = stop;
		this.page = page;
		this.num = num;
		total = 0;
	}
	
	/**
	 * run method will read through the object's section of the 2D users array, checking
	 * to see if the value at the index of page is equal to the int num. If it is, total is
	 * incremented. It will yield to the other threads every fifth of its section. 
	 */
	public void run() {
		int yieldTime = (start-stop)/QueryHandler.numThreads;
		for (int i=start; i<=stop; i++) {
			if (myArray[i][page] == num) {
				total += 1;
			}
			if (i%yieldTime == 0) {
				yield();
			}
		}
	}
	
	/**
	 * getTotal method will return the total users counted by the object. 
	 * @return total
	 */
	public int getTotal() {
		return total;
	}
}
