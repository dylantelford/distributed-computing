package project1;

import static org.junit.Assert.*;

import org.junit.Test;

/**
 * JUnit testing class for the Thread classes. 
 * 
 * Will use the testing constructors in the Thread classes to test their totals 
 * based on the testArray in the MyMain class. 
 * 
 * @author Dylan Telford
 * 
 */
public class ThreadTests {
	
	@Test
	public void test() {
		//test for Thread 1 counting users who viewed Frontpage (all 10)
		Thread1 t1 = new Thread1("test", 0, 9, 0);
		t1.start();
		try {
			t1.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		assertEquals("t1.getTotal() should return 10.", 10, t1.getTotal());
		
		//test for Thread1 counting users who viewed On-Air (none)
		Thread1 t2 = new Thread1("test", 0, 9, 5);
		t2.start();
		try {
			t2.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		assertEquals("t2.getTotal() should return 0.", 0, t2.getTotal());
		
		//test for Thread2 counting number who viewed Frontpage 5 times (9)
		Thread2 t3 = new Thread2("test", 0, 9, 0, 5);
		t3.start();
		try {
			t3.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		assertEquals("t3.getTotal() should return 9.", 9, t3.getTotal());
		
		//test for Thread2 counting number who viewed Travel 6 times (1)
		Thread2 t4 = new Thread2("test", 0, 9, 16, 6);
		t4.start();
		try {
			t4.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		assertEquals("t4.getTotal() should return 1.", 1, t4.getTotal());
		
		//test for Thread3 counting number who viewed Frontpage more than On-Air (10)
		Thread3 t5 = new Thread3("test", 0, 9, 0, 5);
		t5.start();
		try {
			t5.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		assertEquals("t5.getTotal() should return 10.", 10, t5.getTotal());
		
		//test for Thread3 counting number who viewed News more than Tech (3)
		Thread3 t6 = new Thread3("test", 0, 9, 1, 2);
		t6.start();
		try {
			t6.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		assertEquals("t6.getTotal() should return 3.", 3, t6.getTotal());
	}

}
