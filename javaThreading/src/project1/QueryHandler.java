package project1;

import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.Locale;

/**
 * An object of the QueryHandler class will be made by the GUI each time the 
 * user runs a query. 3 constructors exist to accommodate different information
 * being passed, depending on which query needs to be run. The constructor will 
 * call the correct query method, which will in turn call a makeThread method, 
 * which will run the threads and return a total. The query method will update the 
 * private String answer, which the GUI will retrieve using the getAnswer method.
 * 
 * @author Dylan Telford
 *
 */
public class QueryHandler {
	
	private String answer; //will be updated by query methods and accessed via getAnswer method
	public static final int numThreads = 5; //can be changed here to make more/less threads
	private final int numUsers = 989818;
	
	/**
	 * Constructor for an object that will run query 2, requires a user category selection from the GUI. 
	 * 
	 * @param qNum query number
	 * @param selection category selection
	 */
	public QueryHandler(int qNum, String selection) {
		if (qNum == 2) {
			query2(selection);
		} else {
			answer = "mistake made in object creation...";
		}
	}
	
	/**
	 * Constructor for an object that will run either query 1 or query 4. These queries
	 * require a category selection and number of users selection from the GUI. 
	 * 
	 * @param qNum query number
	 * @param selection category selection
	 * @param numUsers number of users entered 
	 */
	public QueryHandler(int qNum, String selection, int num) {
		if (qNum == 1) {
			query1(selection, num);
		} else if (qNum == 4) {
			query4(selection, num);
		} else {
			answer = "mistake made in object creation...";
		}
	}
	
	/**
	 * Constructor for an object that will run either query 3 or query 5. These queries require 
	 * a category X selection and a category Y selection from the GUI. 
	 * 
	 * @param qNum query number
	 * @param selectionX category selection X
	 * @param selectionY category selection for Y
	 */
	public QueryHandler(int qNum, String selectionX, String selectionY) {
		if (qNum == 3) {
			query3(selectionX, selectionY);
		} else if (qNum == 5) {
			query5(selectionX, selectionY);
		} else {
			answer = "mistake made in object creation...";
		}
	}
	
	/**
	 * method for query 1 will call the makeThread1 method which will return a total 
	 * from the threads. By comparing the total to the number of users entered by the 
	 * user, it will update answer to say either yes or no, and show the total. 
	 * 
	 * @param sel category selection
	 * @param num number of users
	 */
	private void query1(String sel, int num) {
		int total = makeThread1(sel);
		
		if (total > num) {
			answer = "Yes, " + NumberFormat.getNumberInstance(Locale.US).format(total) + " users looked at " + sel + ".";
		} else {
			answer = "No, only " + NumberFormat.getNumberInstance(Locale.US).format(total) + "users looked at " + sel + ".";
		}
	}
	
	/**
	 * method for query 2 will call the makeThread1 method to get a total number of users 
	 * that viewed a category selected by the user. It will use the numUsers variable to compute
	 * the percentage of users that viewed that page. It will update answer to show that percentage. 
	 * 
	 * @param sel category selection
	 */
	private void query2(String sel) {
		int total = makeThread1(sel);
		double percent = ((total / (double)numUsers) * 100);
		String percentStr = new DecimalFormat("###.##").format(percent);
		
		answer = percentStr + "% of users viewed " + sel + ".";
	}
	
	/**
	 * method for query 3 will make two calls to the makeThread1 method to get the total views of 
	 * two different categories selected by the user. It will compare these totals and update 
	 * answer to reflect whether more users viewed category X or category Y.
	 * 
	 * @param selX category selection X
	 * @param selY category selection Y
	 */
	private void query3(String selX, String selY) {
		int totalX = makeThread1(selX);
		int totalY = makeThread1(selY);
		
		if(totalX > totalY) {
			answer = "Yes, " + NumberFormat.getNumberInstance(Locale.US).format(totalX) + " users viewed " + 
					selX + " while " + NumberFormat.getNumberInstance(Locale.US).format(totalY) +
					" users viewed " + selY + ".";
		} else{
			answer = "No, only " + NumberFormat.getNumberInstance(Locale.US).format(totalX) + " users viewed " +
					selX + " while " + NumberFormat.getNumberInstance(Locale.US).format(totalY) +
					" users viewed " + selY + ".";
		}
	}

	/**
	 * method for query 4 will make a call to the makeThread2 method, which will return a total of
	 * users who viewed a certain category __ number of times. It will update answer to show that number.
	 * 
	 * @param sel category selection
	 * @param num number of users
	 */
	private void query4(String sel, int num) {
		int total = makeThread2(num, sel);
		
		answer = NumberFormat.getNumberInstance(Locale.US).format(total) + " users viewed " + 
				sel + " " + num + " times.";
	}
	
	/**
	 * method for query 5 will make a call to the makeThread3 method, returning a total of users
	 * who viewed selection x more than selection y. It will then use the numUsers variable to 
	 * compute the percent of users, and update answer to show this percent. 
	 * 
	 * @param selX category selection X
	 * @param selY category selection Y
	 */
	private void query5(String selX, String selY) {
		int total = makeThread3(selX, selY);
		double percent = ((total / (double)numUsers) * 100);
		String percentStr = new DecimalFormat("###.##").format(percent);

		answer = percentStr + "% of users viewed " + selX + " more than " + selY + ".";
	}
	
	/**
	 * method to make instances of Thread1. Used by queries 1, 2, and 3. It will make as many
	 * instances as the numThreads variable says to, passing to each thread a start, stop, and 
	 * category selection to count users by. It will start and join the threads, and then retrieve all
	 * of their totals using the thread's getTotal method. The method will return the total of all 
	 * totals returned by the threads to the query function that called it. 
	 * 
	 * @param sel category selection
	 * @return total users counted by the threads
	 */
	private int makeThread1(String sel) {
		Thread1[] myThreads = new Thread1[numThreads];
		
		for (int i=0; i<numThreads; i++) {
			int begin = i*(numUsers/numThreads);
			int end = begin + (numUsers/numThreads);
			if (i == numThreads-1) {
				end = numUsers-1;
			}
			myThreads[i] = new Thread1(begin, end, sel);
			myThreads[i].start();
		}
		
		for (int i=0; i<numThreads; i++) {
			try {
				myThreads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		int total = 0;
		for (int i=0; i<numThreads; i++) {
			total += myThreads[i].getTotal();
		}
		
		return total;
	}
	
	/**
	 * method to make instances of Thread2. Used by query 4. It will make as many
	 * instances as the numThreads variable says to, passing to each thread a start, stop, user number,  
	 * and category selection to count users by. It will start and join the threads, and then retrieve all
	 * of their totals using the thread's getTotal method. The method will return the total of all 
	 * totals returned by the threads to the query function that called it. 
	 * 
	 * @param userNum number of users 
	 * @param sel category selection
	 * @return total users counted by all threads
	 */
	private int makeThread2(int userNum, String sel) {
		Thread2[] myThreads = new Thread2[numThreads];
		
		for (int i=0; i<numThreads; i++) {
			int begin = i*(numUsers/numThreads);
			int end = begin + (numUsers/numThreads);
			if (i == numThreads-1) {
				end = numUsers-1;
			}
			myThreads[i] = new Thread2(begin, end, userNum, sel);
			myThreads[i].start();
		}
		
		for (int i=0; i<numThreads; i++) {
			try {
				myThreads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		int total = 0;
		for (int i=0; i<numThreads; i++) {
			total += myThreads[i].getTotal();
		}
		
		return total;
	}
	
	/**
	 * method to make instances of Thread3. Used by query 5. It will make as many
	 * instances as the numThreads variable says to, passing to each thread a start, stop, category x, and 
	 * category y selection to count users by. It will start and join the threads, and then retrieve all
	 * of their totals using the thread's getTotal method. The method will return the total of all 
	 * totals returned by the threads to the query function that called it. 
	 * 
	 * @param catX category X selection
	 * @param catY category Y selection
	 * @return total number of users counted by all threads
	 */
	private int makeThread3(String catX, String catY) {
		Thread3[] myThreads = new Thread3[numThreads];
		
		for (int i=0; i<numThreads; i++) {
			int begin = i*(numUsers/numThreads);
			int end = begin + (numUsers/numThreads);
			if (i == numThreads-1) {
				end = numUsers-1;
			}
			myThreads[i] = new Thread3(begin, end, catX, catY);
			myThreads[i].start();
		}
		
		for (int i=0; i<numThreads; i++) {
			try {
				myThreads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		int total = 0;
		for (int i=0; i<numThreads; i++) {
			total += myThreads[i].getTotal();
		}
		
		return total;
	}
	
	/**
	 * getAnswer method will return the String answer to the GUI to be displayed to the user. 
	 * @return answer
	 */
	public String getAnswer() {
		return answer;
	}
	
}
