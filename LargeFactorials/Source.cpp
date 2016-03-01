/*
	PROBLEM: Compute N!, allowing for 1 <= N <= 100.
				Note that for values of N > 20 can't be stored even in a 64-bit variable.

	SOLUTION: Construct the number as a string, performing the multiplication manually.
*/

#include <vector>
#include <iostream>
using namespace std;

//	Computes n factorial, storing the answer in "result".
void Factorial(const unsigned int n, vector<unsigned int>& result);

//	Computes product of some integer curN and some potentially large integer num.
void Multiply(const unsigned int curN, vector<unsigned int>& num);

int main() {
	//  Input the parameter.
	int n;
	cin >> n;

	//  Calculate the result.
	vector<unsigned int> result;
	Factorial(n, result);

	//  Output the result.
	for (int i = result.size()-1; i >= 0; --i) {
		cout << result[i];
	}
	cout << endl;

	return 0;
}

void Factorial(const unsigned int n, vector<unsigned int> &result) {
	//	Initialize result to "1".
	result.clear();
	result.push_back(1);

	//	Factorial: 1 x 2 x ... x n-1 x n
	for (unsigned int curN = 2; curN <= n; ++curN) {
		Multiply(curN, result);
	}

}

//	Note: number is constructed backwards.
//		For example, 42 is stored as num[0] = 2, num[1] = 4
void Multiply(const unsigned int curN, vector<unsigned int>& num) {
	char carry = 0;
	
	//	Multiply curN with each digit of num, carrying values greater than 10 forward to the next iteration.
	for (unsigned int i = 0; i < num.size(); ++i) {
		unsigned int product = (num[i] * curN) + carry;
		num[i] = product % 10;
		carry = product / 10;	//	Intentional integer division.
	}

	//	Apply any remaining carry to the final value.
	while (carry > 0) {
		num.push_back(carry % 10);
		carry /= 10;			//	Intentional integer division.
	}
}