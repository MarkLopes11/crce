import java.util.*;

class TwoDArrray{
	public static void main(String args[]){
	Scanner sc = new Scanner(System.in);
		System.out.print("Enter the 2D array size :\t");
		int row = sc.nextInt();
		int col = sc.nextInt();
		
		System.out.println("Enter the elements of the 2D array");
		
		int twoD[][] = new int[row][col];
		
		for(int i = 0; i < row;  i++){
			for(int j = 0; j < col;  j++){
				twoD[i][j] = sc.nextInt();
			}
		}
		
		for(int x[]: twoD){
			for(int y : x){
				System.out.print(y + "   ");
			}
			System.out.println();
			System.out.println();		
		}
	}
}
