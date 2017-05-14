#include "largeNum.h"


//a bunch of useful numbers which can be used in calculations
const largeNum zero(0);
const largeNum negZero = -zero;
const largeNum one(1);
const largeNum two(2);
const largeNum ten(10);

/*################################
  ##########CONSTRUCTORS##########
  ################################*/

largeNum::largeNum(                   )
{
	value.resize(1);
	value[0] = 0;
	sign = '+';
}

largeNum::largeNum(std::string s_input)
{
	sign = '+';
	uint i = 1;
	if (s_input[0] == '-') sign = '-';
	else i--;
	value.resize(s_input.size());
	for (; i < s_input.size(); i++)
	{
		value.at(i) = resolveChar(s_input[i]);
	}
}

largeNum::largeNum(char c_input       )
{
	value.resize(1);
	value.at(0) = resolveChar(c_input);
	sign = '+';
}

largeNum::~largeNum(                  )
{
}

/*#######################################
  ##########SETTERS AND GETTERS##########
  #######################################*/

std::vector<int>& largeNum::getValue() const	
{
	return const_cast<largeNum*>(this)->value;
}

char largeNum::getSign() const
{
	return this->sign;
}

void largeNum::setValue(const std::vector<int>& input)
{
	value = input;
}

/*##########################
  ##########ASSETS##########
  ##########################*/

int largeNum::compare(const largeNum& toTest) const
{
	std::vector<int> tmp1 = this->getValue();
	std::vector<int> tmp2 = toTest.getValue();

	while (tmp1.at(0) == 0 && tmp1.size() != 1) tmp1.erase(tmp1.begin());
	while (tmp2.at(0) == 0 && tmp2.size() != 1) tmp2.erase(tmp2.begin());

	if (tmp1.size() > tmp2.size()) return 1;
	if (tmp1.size() < tmp2.size()) return -1;
	uint size = tmp1.size();

	for (uint i = 0; i < size; i++)
	{
		if (tmp1.at(i) > tmp2.at(i)) return 1;
		if (tmp1.at(i) < tmp2.at(i)) return -1;
	}
	return 0;
}

largeNum largeNum::removeZerosAtStart() const
{
	std::vector<int>& sVal = this->getValue();
	while (sVal.at(0) == 0 && sVal.size() != 1) sVal.erase(sVal.begin());
	return *this;
}

largeNum& largeNum::changeSign()
{
	if (this->sign == '+') this->sign = '-';
	else this->sign = '+';
	return *this;
}

largeNum& largeNum::toPositive()
{
	if (this->sign == '-') this->changeSign();
	return *this;
}

largeNum& largeNum::toNegative()
{
	if (this->sign == '+') this->changeSign();
	return *this;
}

/*#################################
  ##########I-O OPERATORS##########
  #################################*/

std::istream& operator >>(std::istream& is, largeNum& val            )
{
	//takes a string as input then 
	//makes a number out of that stored in a vector
	std::string s_input;
	uint i = 0;
	val.sign = '+';
	is >> s_input;
	if (s_input[0] == '-')
	{
		val.sign = '-';
		s_input.erase(s_input.begin());
	}
	if (s_input[0] > 57 || s_input[0] < 48) throw "Error, please enter only one minus sign!\n";
	std::vector<int> futureValue(s_input.size());
	try
	{
		for (; i < s_input.size(); i++)
		{
			futureValue.at(i) = resolveChar(s_input[i]);
		}
		val.setValue(futureValue);
		val.removeZerosAtStart();
		return is;
	}
	catch (const char* msg)
	{
		std::cout << msg;
		return is;
	}
}

std::ostream& operator <<(std::ostream& os, const largeNum& outputVal)
{
	if (outputVal.sign == '-') std::cout << '-';
	outputVal.removeZerosAtStart();
	for (uint i = 0; i < outputVal.getValue().size(); i++)
	{
		os << outputVal.getValue().at(i);
	}
	return os;
}

/*##########################################
  ##########MATHEMATICAL FUNCTIONS##########
  ##########################################*/

largeNum largeNum::factorial()
{
	largeNum res = one;
	while (*this > one)
	{
		res *= *this;
		*this -= one;
	}
	return res;
}

/*########################################
  ##########ARITHMETIC OPERATORS##########
  ########################################*/

largeNum largeNum::operator+(const largeNum& summand   ) const
{
	largeNum returnNum;
	//macros to shorten the code
	largeNum LNtmp1 = *this;
	largeNum LNtmp2 = summand;
	std::vector<int>& tmp1 = LNtmp1.getValue();
	std::vector<int>& tmp2 = LNtmp2.getValue();
	int size;
	bool storedTen = false;
	int temp;
	if (LNtmp1 == zero) return summand;
	if (LNtmp2 == zero) return *this;
	//adapts the smaller vector to the larger vector by resizing him and reversing him so 1 turns into 00...01
	if (tmp1.size() > tmp2.size())
	{
		//puts as many 0s in front of the number as there are more digits in the other number
		size = tmp1.size();
		while (tmp1.size() != tmp2.size())
		{
			tmp2.insert(tmp2.begin(), 0);
		}
	}
	else if (tmp2.size() > tmp1.size())
	{
		//puts as many 0s in front of the number as there are more digits in the other number
		size = tmp2.size();
		while (tmp2.size() != tmp1.size())
		{
			tmp1.insert(tmp1.begin(), 0);
		}
	}
	else size = tmp1.size();
	std::vector<int> returnValue(size + 1);
	// if a digit + a digit exceeds 10 this gets stored to be added to the next comparison
	if (LNtmp1.sign == '+' && LNtmp2.sign == '+')
	{
		//adds all numbers together from right to left
		for (int i = size - 1; i >= 0; i--)
		{
			//checks whether the addition exceeds 10 to store in "storedTen"
			if (tmp1.at(i) + tmp2.at(i) + storedTen > 9)
			{
				temp = tmp1.at(i) + tmp2.at(i) + storedTen - 10;
				storedTen = true;
			}
			else
			{
				temp = tmp1.at(i) + tmp2.at(i) + storedTen;
				storedTen = false;
			}
			returnValue.at(i + 1) = temp;
		}
		//should the last addition have been larger than 10 the end number will have to be increased by one more digit 
		//e.g. 5+5 = 10
		if (storedTen == 1) returnValue[0] = 1;
		returnNum.setValue(returnValue);
	}
	else if (LNtmp1.sign == '-' && LNtmp2.sign == '-')
	{
		returnNum.sign = '-';
		//adds all numbers together from right to left
		for (int i = size - 1; i >= 0; i--)
		{
			//checks whether the addition exceeds 10 to store in "storedTen"
			if (tmp1.at(i) + tmp2.at(i) + storedTen > 9)
			{
				temp = tmp1.at(i) + tmp2.at(i) + storedTen - 10;
				storedTen = true;
			}
			else
			{
				temp = tmp1.at(i) + tmp2.at(i) + storedTen;
				storedTen = false;
			}
			returnValue.at(i + 1) = temp;
		}
		if (returnValue.at(0) == 0) returnValue.erase(returnValue.begin());
		if (storedTen == 1) returnValue.insert(returnValue.begin(), 1);
		returnNum.setValue(returnValue);
	}
	//case for when the signs are different
	else
	{
		if (LNtmp1.compare(LNtmp2) == 0) return zero; // -x + x = 0
		if (LNtmp1.compare(LNtmp2) == 1) returnNum.sign = LNtmp1.sign;
		else returnNum.sign = LNtmp2.sign;
		//for more information on how this part works check: https://www.youtube.com/watch?v=PS5p9caXS4U
		//turns the in the video described method, summand which will function as the subtrahend and changes it as required
		//with the difference that it doesn't reduce it by 10 at the end since we will invert the result
		if (LNtmp1.sign == '-')
		{
			for (int i = 0; i < size; i++)
			{
				//in either case if the digit is 0 it must not be inverted due to us using negative numbers
				if (i == size - 1)
				{
					tmp1.at(i) = (10 - tmp1.at(i));
					break;
				}
				tmp1.at(i) = (9 - tmp1.at(i));
			}
		}
		else
		{
			if (LNtmp2 > LNtmp1) returnNum.changeSign();
			largeNum tmp = LNtmp1;
			LNtmp1 = LNtmp2;
			LNtmp2 = tmp;
			for (int i = 0; i < size; i++)
			{
				if (i == size - 1)
				{
					tmp1.at(i) = (10 - tmp1.at(i));
					break;
				}
				tmp1.at(i) = (9 - tmp1.at(i));
			}
		}
		//adds newly created vector with the original summand
		for (int i = size - 1; i >= 0; i--)
		{
			//checks whether the addition exceeds 10 to store in "storedTen"
			if (tmp1.at(i) + tmp2.at(i) + storedTen > 9)
			{
				temp = tmp1.at(i) + tmp2.at(i) + storedTen - 10;
				storedTen = true;
			}
			else
			{
				temp = tmp1.at(i) + tmp2.at(i) + storedTen;
				storedTen = false;
			}
			returnValue.at(i + 1) = temp;
		}
		if (storedTen == true) returnValue.insert(returnValue.begin(), 1);
		while (returnValue.at(0) == 0 && returnValue.size() != 1) returnValue.erase(returnValue.begin());
		//in case the first summand is negative and smaller than the second or the second summand is the first digit gets removed
		//otherwise the first digit has to stay due to negative numbers
		//method doesn't work for negative numbers so we adapt it
		//hereby we take the invert of the values
		//since we only want to do 10-.at(i) we check whether it's already been done
		if (returnNum.sign == '-')
		{
			bool addedTen = false;
			for (int i = (returnValue.size() - 1); i >= 0; i--)
			{
				if (returnValue.at(i) == 0 && addedTen == false) continue;
				if (addedTen == false)
				{
					returnValue.at(i) = 10 - returnValue.at(i);
					addedTen = true;
				}
				//inverts the result because of negativity
				else
				{
					returnValue.at(i) = 9 - returnValue.at(i);
				}
				if (addedTen == false)returnValue.at(returnValue.size() - 1)++;
			}
		}
		if (returnNum.sign == '+')
		{
			if (tmp1.size() > 1 && tmp2.size() > 1) returnValue.insert(returnValue.begin(), 1);
			returnValue.erase(returnValue.begin());
		}
		if (returnNum.sign == '+') returnValue.erase(returnValue.begin());
	}
	//gets rid of all 0s in front;
	while (returnValue.at(0) == 0 && returnValue.size() != 1) returnValue.erase(returnValue.begin());
	returnNum.setValue(returnValue);
	return returnNum;
}

largeNum largeNum::operator-(const largeNum& subtrahend) const
{
	//since we basically defined subtraction and negative numbers in the + operator 
	//and subtraction is the counterpart to addition we just return the addition of the
	//minuend and the negative subtrahend
	largeNum result = *this;
	return result + -subtrahend;
}

largeNum largeNum::operator*(const largeNum& factor    ) const
{
	if (*this == zero || factor == zero || *this == negZero || factor == negZero) return zero;
	largeNum LNtmp1 = *this;
	largeNum LNtmp2 = factor;
	largeNum result = zero;
	//if you multiply e.g. 4 with 4 you get 16 so 6 carryover 1
	int carryOver = 0;
	largeNum toAdd = zero;
	//macros
	std::vector<int>& tmp1 = LNtmp1.getValue();
	std::vector<int>& tmp2 = LNtmp2.getValue();
	std::vector<int>& tmp3 = toAdd.getValue();
	std::vector<int>& tmp4 = result.getValue();
	//goes through each digit from factor 2 and multiplies it with each digit from factor 1 adding the values together
	for (int i = tmp2.size() - 1; i >= 0; i--)
	{
		for (int j = tmp1.size() - 1; j >= 0; j--)
		{
			int temp = tmp1.at(j) * tmp2.at(i) + carryOver;
			carryOver = temp / 10;
			tmp3.insert(tmp3.begin(), temp % 10);
		}
		//to adapt since the digits in this kind of multiplication are technically shifted.
		int zeroExtender = tmp2.size() - 1 - i;
		while (zeroExtender != 0)
		{
			tmp3.insert(tmp3.end(), 0);
			zeroExtender--;
		}
		//in case there's still a value left over this value becomes the new 1. digit
		tmp3.insert(tmp3.begin(), carryOver);
		//then it adds the previously calculated result to the current one and resets it
		result += toAdd;
		carryOver = 0;
		toAdd = zero;
	}
	//afterwards the final digit gets added if there is one.
	//tmp4.insert(tmp4.begin(), carryOver);
	//the last digit gets removed because there is always a 0 at the end
	tmp4.erase(tmp4.end() - 1);
	if (tmp4.size() == 0) return zero;
	//since multiplication disregards signs when it comes to value
	//the end sign is determined by the input signs though.
	if (this->sign == factor.sign) result.sign = '+';
	else result.sign = '-';
	return result;
}

largeNum largeNum::operator/(const largeNum& divisor   ) const
{
	if (this->compare(divisor) == -1) throw "Fractions or float-point numbers are not yet supported!\n";
	if (*this == divisor) return one;
	if (divisor == zero) throw "You can't divide by 0!\n";
	if (divisor == one) return *this; // x/1 = x

	//macros and declarations

	//there are used in the calculations
	largeNum tmp1		 = const_cast<largeNum&>(*this);
	largeNum tmp2		 = const_cast<largeNum&>(divisor);

	//compareVals are used to identify for how long the loop would have to run
	largeNum compareVal1 = const_cast<largeNum&>(*this);
	largeNum compareVal2 = const_cast<largeNum&>(divisor);

	//the result and a temporary which is used to drag down digits
	largeNum result		 = const_cast<largeNum&>(zero);
	largeNum temporary   = const_cast<largeNum&>(zero);

	//iterates through the given divident
	long long iterator   = 0;

	//if more than 1 number is dragged down a 0 is appended 
	long long dragCount  = 0;

	//the current value which will then be inserted
	int currResult       = 0;

	std::vector<int>& divValue = tmp1.getValue();
	std::vector<int>& rValue   = result.getValue();
	std::vector<int>& tempVal  = temporary.getValue();

	tmp1.toPositive();
	tmp2.toPositive();
	compareVal1.toPositive();
	compareVal2.toPositive();
	//goes from left to right subtracting every time and then adding a result	
	while (result * compareVal2 < compareVal1 - compareVal2 + one)
	{
		//drags down so many digits until it can actually be subtracted
		while (temporary < tmp2)
		{
			//to prevent out of range when having calculations with 0s in the divident
			if (divValue.at(0) == 0 && divValue.size() == iterator) break;
			if (dragCount >= 1)
			{
				rValue.push_back(0);
			}
			tempVal.push_back(divValue.at(iterator));
			dragCount++;
			iterator++;
		}
		dragCount = 0;
		temporary.removeZerosAtStart();
		//removes the digits from the first number corresponding to the amount of digits added to the temporary one
		for (uint i = 0; i < tempVal.size(); i++)
		{
			if (divValue.size() == 1)break;
			divValue.erase(divValue.begin());
		}

		//subtracts the divisor from the temporary result and adds a corresponding result every time.
		while (temporary - tmp2 >= zero)
		{
			if (tmp2 == zero) break;
			temporary -= tmp2;
			currResult++;
		}
		//inserts the previously calculated result in the end result
		rValue.insert(rValue.end(), currResult);
		currResult = 0;
		iterator = 0;
	}
	result.removeZerosAtStart();
	if (this->sign == divisor.sign) result.sign = '+';
	else result.sign = '-';
	return result;
}

largeNum largeNum::operator%(const largeNum& divisor   ) const
{
	//returns the reminder of a division
	return *this - (*this / divisor)*divisor;
}

/*#############################################
  ##########OPERATOR-EQUALS OPERATORS##########
  #############################################*/

largeNum largeNum::operator+=(const largeNum& summant   )
{
	*this = *this + summant;
	return *this;
}

largeNum largeNum::operator-=(const largeNum& subtrahend)
{
	*this = *this - subtrahend;
	return *this;
}

largeNum largeNum::operator*=(const largeNum& factor    )
{
	*this = *this * factor;
	return *this;
}

largeNum largeNum::operator/=(const largeNum& divisor   )
{
	*this = *this / divisor;
	return *this;
}

largeNum largeNum::operator%=(const largeNum& divisor   )
{
	*this = *this % divisor;
	return *this;
}

/*#######################################
  ##########INCREMENT OPERATORS##########
  #######################################*/

largeNum largeNum::operator++(int x)
{
	largeNum tmp(*this);
	*this += one;
	return tmp;
}

largeNum largeNum::operator--(int x)
{
	largeNum tmp(*this);
	*this -= one;
	return tmp;
}

//TODO 
//check this pls not sure about that "int x"

largeNum largeNum::operator -(     )
{
	// While not changing the Numbers value it does change the sign so
	largeNum result = *this;
	result.changeSign();
	return result;
}

largeNum largeNum::operator++(     )
{
	*this += one;
	return *this;
}

largeNum largeNum::operator--(     )
{
	*this -= one;
	return *this;
}

largeNum largeNum::operator -(     ) const
{
	largeNum result = const_cast<largeNum&>(*this);
	result.changeSign();
	return result;
}

/*########################################
  ##########COMPARISON OPERATORS###########
  ########################################*/

bool largeNum::operator == (const largeNum& test) const
{
	if (this->sign != test.sign) return false;
	return (this->compare(test) == 0) ? true : false;
}

bool largeNum::operator >  (const largeNum& test) const
{
	if (this->sign == '+' && test.sign == '-') return true;
	if (this->sign != test.sign) return false;
	if (this->sign == '+' && test.sign == '+')
	{
		if (this->compare(test) == 0) return false;
		if (this->compare(test) == -1) return false;
		return true;
	}
	if (this->compare(test) == 0) return false;
	if (this->compare(test) == 1) return false;
	return true;
}

bool largeNum::operator <  (const largeNum& test) const
{
	if (this->sign == '-' && test.sign == '+') return true;
	if (this->sign != test.sign) return false;
	if (this->sign == '+' && test.sign == '+')
	{
		if (this->compare(test) == 0) return false;
		if (this->compare(test) == 1) return false;
		return true;
	}
	if (this->compare(test) == 0) return false;
	if (this->compare(test) == -1) return false;
	return true;
}

bool largeNum::operator >= (const largeNum& test) const
{
	if (*this == test) return true;
	if (*this > test) return true;
	return false;
}

bool largeNum::operator <= (const largeNum& test) const
{
	if (*this == test) return true;
	if (*this < test) return true;
	return false;
}

bool largeNum::operator != (const largeNum& test) const
{
	if (*this == test) return false;
	return true;
}

bool largeNum::operator !() const
{
	if (this->getValue().size() == 0 && this->getValue().at(0) != 0) return false;
	return true;
}