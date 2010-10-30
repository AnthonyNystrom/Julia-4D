#include "stdafx.h"

#include "eqparse.h"
#include "utilities.h"

#include <string>
using std::string;

#include <sstream>
using std::ostringstream;

// lala: for debug to screen purposes only
#include <iostream>
using std::cout;
using std::endl;



qmath_func_ptr quaternion_julia_set_equation_parser::GetFunctionInstruction(const string &src_token)
{
	string temp = lower_string(src_token);

	for(size_t i = 0; i < function_map.size(); i++)
		if(function_map[i].function_name == temp)
			return function_map[i].f;

	return 0;
}



void quaternion_julia_set_equation_parser::SetupFunctionMap(void)
{
	function_map.push_back(function_mapping("sin", &quaternion_math_base<double>::sin));
	function_map.push_back(function_mapping("sinh", &quaternion_math_base<double>::sinh));
	function_map.push_back(function_mapping("exsin", &quaternion_math_base<double>::exsin));
	function_map.push_back(function_mapping("exsinh", &quaternion_math_base<double>::exsinh));
	function_map.push_back(function_mapping("coversin", &quaternion_math_base<double>::coversin));
	function_map.push_back(function_mapping("coversinh", &quaternion_math_base<double>::coversinh));

	function_map.push_back(function_mapping("cos", &quaternion_math_base<double>::cos));
	function_map.push_back(function_mapping("cosh", &quaternion_math_base<double>::cosh));
	function_map.push_back(function_mapping("excos", &quaternion_math_base<double>::excos));
	function_map.push_back(function_mapping("excosh", &quaternion_math_base<double>::excosh));
	function_map.push_back(function_mapping("versin", &quaternion_math_base<double>::versin));
	function_map.push_back(function_mapping("versinh", &quaternion_math_base<double>::versinh));
//	add
//	sub
	function_map.push_back(function_mapping("ln", &quaternion_math_base<double>::ln));
	function_map.push_back(function_mapping("exp", &quaternion_math_base<double>::exp));
	function_map.push_back(function_mapping("sqrt", &quaternion_math_base<double>::sqrt));
	function_map.push_back(function_mapping("inverse", &quaternion_math_base<double>::inverse));
	function_map.push_back(function_mapping("conjugate", &quaternion_math_base<double>::conjugate));
//	mul
//	pow
//	pow_dynamic
//	div
	function_map.push_back(function_mapping("sinc", &quaternion_math_base<double>::sinc));
	function_map.push_back(function_mapping("sinhc", &quaternion_math_base<double>::sinhc));
	function_map.push_back(function_mapping("csc", &quaternion_math_base<double>::csc));
	function_map.push_back(function_mapping("csch", &quaternion_math_base<double>::csch));
	function_map.push_back(function_mapping("excsc", &quaternion_math_base<double>::excsc));
	function_map.push_back(function_mapping("excsch", &quaternion_math_base<double>::excsch));
	function_map.push_back(function_mapping("covercsc", &quaternion_math_base<double>::covercsc));
	function_map.push_back(function_mapping("covercsch", &quaternion_math_base<double>::covercsch));
	function_map.push_back(function_mapping("cscc", &quaternion_math_base<double>::cscc));
	function_map.push_back(function_mapping("cschc", &quaternion_math_base<double>::cschc));
	function_map.push_back(function_mapping("cosc", &quaternion_math_base<double>::cosc));
	function_map.push_back(function_mapping("coshc", &quaternion_math_base<double>::coshc));
	function_map.push_back(function_mapping("sec", &quaternion_math_base<double>::sec));
	function_map.push_back(function_mapping("sech", &quaternion_math_base<double>::sech));
	function_map.push_back(function_mapping("exsec", &quaternion_math_base<double>::exsec));
	function_map.push_back(function_mapping("exsech", &quaternion_math_base<double>::exsech));
	function_map.push_back(function_mapping("vercsc", &quaternion_math_base<double>::vercsc));
	function_map.push_back(function_mapping("vercsch", &quaternion_math_base<double>::vercsch));
	function_map.push_back(function_mapping("secc", &quaternion_math_base<double>::secc));
	function_map.push_back(function_mapping("sechc", &quaternion_math_base<double>::sechc));
	function_map.push_back(function_mapping("tan", &quaternion_math_base<double>::tan));
	function_map.push_back(function_mapping("tanh", &quaternion_math_base<double>::tanh));
	function_map.push_back(function_mapping("extan", &quaternion_math_base<double>::extan));
	function_map.push_back(function_mapping("extanh", &quaternion_math_base<double>::extanh));
	function_map.push_back(function_mapping("covertan", &quaternion_math_base<double>::covertan));
	function_map.push_back(function_mapping("covertanh", &quaternion_math_base<double>::covertanh));
	function_map.push_back(function_mapping("tanc", &quaternion_math_base<double>::tanc));
	function_map.push_back(function_mapping("tanhc", &quaternion_math_base<double>::tanhc));
	function_map.push_back(function_mapping("cot", &quaternion_math_base<double>::cot));
	function_map.push_back(function_mapping("coth", &quaternion_math_base<double>::coth));
	function_map.push_back(function_mapping("excot", &quaternion_math_base<double>::excot));
	function_map.push_back(function_mapping("excoth", &quaternion_math_base<double>::excoth));
	function_map.push_back(function_mapping("covercot", &quaternion_math_base<double>::covercot));
	function_map.push_back(function_mapping("covercoth", &quaternion_math_base<double>::covercoth));
	function_map.push_back(function_mapping("cotc", &quaternion_math_base<double>::cotc));
	function_map.push_back(function_mapping("cothc", &quaternion_math_base<double>::cothc));

	}





bool quaternion_julia_set_equation_parser::is_valid_destination(const string &src_string)
{
	string temp = lower_string(src_string);

	if(temp.size() == 1)
	{
		if(temp == "z")
			return true;
		else
			return false;
	}
	else if(temp.size() > 2 && temp.size() < 7)
	{
		if(temp.substr(0, 2) != "z.")
			return false;

		temp.erase(0, 2);

		for(size_t i = 0; i < temp.size(); i++)
		{
			if(temp[i] != 'x' && temp[i] != 'y' && temp[i] != 'z' && temp[i] != 'w')
			{
				return false;
			}
			else
			{
				// catch stuff like .yx and .yy and (incorrect order and dupes)... correct order is: xyzw (not wxyz)
				for(size_t j = 0; j < i; j++)
				{
					int temp_i_val = 0;
					int temp_j_val = 0;

					if(temp[i] == 'x')
						temp_i_val = 0;
					else if(temp[i] == 'y')
						temp_i_val = 1;
					else if(temp[i] == 'z')
						temp_i_val = 2;
					else if(temp[i] == 'w')
						temp_i_val = 3;

					if(temp[j] == 'x')
						temp_j_val = 0;
					else if(temp[j] == 'y')
						temp_j_val = 1;
					else if(temp[j] == 'z')
						temp_j_val = 2;
					else if(temp[j] == 'w')
						temp_j_val = 3;

					if(temp_i_val <= temp_j_val)
						return false;
				}
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool quaternion_julia_set_equation_parser::is_variable_swizzle(const string &src_string)
{
	string temp = lower_string(src_string);

	if(temp.size() > 2 && temp.size() < 7)
	{
		string temp_prefix = temp.substr(0, 2);

		if(temp_prefix != "z.")
			return false;

		temp.erase(0, 2);

		for(size_t i = 0; i < temp.size(); i++)
			if( temp[i] != 'x' && temp[i] != 'y' && temp[i] != 'z' && temp[i] != 'w')
				return false;
	}
	else
	{
		return false;
	}

	return true;
}

bool quaternion_julia_set_equation_parser::is_constant_swizzle(const string &src_string)
{
	string temp = lower_string(src_string);

	if(temp.size() > 2 && temp.size() < 7)
	{
		string temp_prefix = temp.substr(0, 2);

		if(temp_prefix != "c.")
			return false;

		temp.erase(0, 2);

		for(size_t i = 0; i < temp.size(); i++)
			if( temp[i] != 'x' && temp[i] != 'y' && temp[i] != 'z' && temp[i] != 'w')
				return false;
	}
	else
	{
		return false;
	}

	return true;
}

bool quaternion_julia_set_equation_parser::is_raw_swizzle(const string &src_string)
{
	string temp = lower_string(src_string);

	if(temp.size() > 1 && temp.size() < 6)
	{
		if(temp[0] != '.')
			return false;

		temp.erase(0, 1);

		for(size_t i = 0; i < temp.size(); i++)
			if( temp[i] != 'x' && temp[i] != 'y' && temp[i] != 'z' && temp[i] != 'w')
				return false;
	}
	else
	{
		return false;
	}

	return true;
}


bool quaternion_julia_set_equation_parser::is_function(const string &src_string)
{
	string temp = lower_string(src_string);

	if("quat" == temp)
		return true;

	for(size_t i = 0; i < function_map.size(); i++)
		if(temp == function_map[i].function_name)
			return true;


	return false;

	/*
	if( temp == "sin" || temp == "sinh" || temp == "exsin" || temp == "exsinh" || temp == "coversin" || temp == "coversinh" || temp == "sinc" || temp == "sinhc" ||
		temp == "csc" || temp == "csch" || temp == "excsc" || temp == "excsch" || temp == "covercsc" || temp == "covercsch" || temp == "cscc" || temp == "cschc" ||
		temp == "cos" || temp == "cosh" || temp == "excos" || temp == "excosh" || temp == "versin" || temp == "versinh" || temp == "cosc" || temp == "coshc" ||
		temp == "sec" || temp == "sech" || temp == "exsec" || temp == "exsech" || temp == "vercsc" || temp == "vercsch" || temp == "secc" || temp == "sechc" ||
		temp == "tan" || temp == "tanh" || temp == "extan" || temp == "extanh" || temp == "covertan" || temp == "covertanh" || temp == "tanc" || temp == "tanhc" ||
		temp == "cot" || temp == "coth" || temp == "excot" || temp == "excoth" || temp == "covercot" || temp == "covercoth" || temp == "cotc" || temp == "cothc" ||
		temp == "sqrt" || temp == "ln" || temp == "exp" || temp == "quat")
	{
		return true;
	}

	return false;
	*/
}

bool quaternion_julia_set_equation_parser::is_math_operator(const string &src_string)
{
	if( src_string == "^" ||
		src_string == "*" ||
		src_string == "/" ||
		src_string == "+" ||
		src_string == "-" )
	{
		return true;
	}

	return false;
}

bool quaternion_julia_set_equation_parser::is_constant(const string &src_string)
{
	string temp = lower_string(src_string);

	if( temp == "c" ||
		temp == "a" )
	{
		return true;
	}
	//	else if(is_constant_swizzle(temp))
	//	{
	//		return true;
	//	}

	return false;
}

bool quaternion_julia_set_equation_parser::is_variable(const string &src_string)
{
	string temp = lower_string(src_string);

	if(temp == "z")
	{
		return true;
	}
	//	else if(is_variable_swizzle(temp))
	//	{
	//		return true;
	//	}

	return false;
}


bool quaternion_julia_set_equation_parser::is_real_number(const string &src_string)
{
	//1
	//23e4
	//1.E2
	//2.717
	//.31415e1
	//7.53e-9
	//7.53e+9

	if(src_string == "")
		return false;

	string temp = lower_string(src_string);

	bool found_dot = false;
	bool found_e = false;
	bool found_digit = false;

	for(size_t i = 0; i < temp.size(); i++)
	{
		if(isdigit(temp[i]))
		{
			if(found_digit == false)
				found_digit = true;	
		}
		else if(temp[i] == 'e')
		{
			if(found_e == true || found_digit == false || i == temp.size() - 1)
				return false;
			else
				found_e = true;
		}
		else if(temp[i] == '-' || temp[i] == '+')
		{
			if(!(i == 0 || temp[i-1] == 'e') || i == temp.size() - 1)
				return false;
		}
		else if(temp[i] == '.')
		{
			if(found_dot == true || (i != 0 && temp[i-1] == 'e'))
				return false;
			else
				found_dot = true;
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool quaternion_julia_set_equation_parser::is_constant_quaternion(const string &src_string)
{
	if(src_string == "")
		return false;

	string temp = lower_string(src_string);

	// look for commas, split into multiple parts, scan each part to see if it's a real number
	vector<string> reals;
	reals.push_back("");

	// split into seperate reals, assuming ',' is used to differentiate between the quaternion's parts
	for(size_t i = 0; i < temp.size(); i++)
	{
		if(temp[i] != ',')
		{
			reals[reals.size() - 1] += temp[i];
		}
		else
		{
			reals.push_back("");
		}
	}

	if(reals.size() != 4)
		return false;
	/*
	// quaternion must have min 1 part, max 4
	if(reals.size() < 1 || reals.size() > 4)
	return false;

	// if less than 4, add additional parts to make it 4 parts
	for(size_t i = reals.size(); i < 4; i++)
	reals.push_back("");
	*/

	// test that each number is a real number literal
	for(size_t i = 0; i < reals.size(); i++)
	{
		if(reals[i] == "")
			reals[i] = "0.0";

		if(false == is_real_number(reals[i]))
		{
			if( lower_string(reals[i]) != "c.x" &&
				lower_string(reals[i]) != "c.y" &&
				lower_string(reals[i]) != "c.z" &&
				lower_string(reals[i]) != "c.w" &&
				lower_string(reals[i]) != "a" &&
				lower_string(reals[i]) != "-c.x" &&
				lower_string(reals[i]) != "-c.y" &&
				lower_string(reals[i]) != "-c.z" &&
				lower_string(reals[i]) != "-c.w" &&
				lower_string(reals[i]) != "-a")
			{
				return false;
			}
		}
	}

	return true;
}

bool quaternion_julia_set_equation_parser::is_variable_quaternion(const string &src_string)
{
	if(src_string == "")
		return false;

	string temp = lower_string(src_string);

	// look for commas, split into multiple parts, scan each part to see if it's a real number
	vector<string> reals;
	reals.push_back("");

	// split into seperate reals, assuming ',' is used to differentiate between the quaternion's parts
	for(size_t i = 0; i < temp.size(); i++)
	{
		if(temp[i] != ',')
		{
			reals[reals.size() - 1] += temp[i];
		}
		else
		{
			reals.push_back("");
		}
	}

	if(reals.size() != 4)
		return false;
	/*
	// quaternion must have min 1 part, max 4
	if(reals.size() < 1 || reals.size() > 4)
	return false;

	// if less than 4, add additional parts to make it 4 parts
	for(size_t i = reals.size(); i < 4; i++)
	reals.push_back("");
	*/

	// if less than 4, add additional parts to make it 4 parts
	for(size_t i = reals.size(); i < 4; i++)
		reals.push_back("");

	bool found_var = false;

	// test that each number is a real number literal
	for(size_t i = 0; i < reals.size(); i++)
	{
		if(reals[i] == "")
			reals[i] = "0.0";

		if(false == is_real_number(reals[i]))
		{
			if( lower_string(reals[i]) != "c.x" &&
				lower_string(reals[i]) != "c.y" &&
				lower_string(reals[i]) != "c.z" &&
				lower_string(reals[i]) != "c.w" &&
				lower_string(reals[i]) != "z.x" &&
				lower_string(reals[i]) != "z.y" &&
				lower_string(reals[i]) != "z.z" &&
				lower_string(reals[i]) != "z.w" &&
				lower_string(reals[i]) != "a" &&
				lower_string(reals[i]) != "-c.x" &&
				lower_string(reals[i]) != "-c.y" &&
				lower_string(reals[i]) != "-c.z" &&
				lower_string(reals[i]) != "-c.w" &&
				lower_string(reals[i]) != "-z.x" &&
				lower_string(reals[i]) != "-z.y" &&
				lower_string(reals[i]) != "-z.z" &&
				lower_string(reals[i]) != "-z.w" &&
				lower_string(reals[i]) != "-a")
			{
				return false;
			}
			else
			{
				if(	lower_string(reals[i]) == "z.x" ||
					lower_string(reals[i]) == "z.y" ||
					lower_string(reals[i]) == "z.z" ||
					lower_string(reals[i]) == "z.w" ||
					lower_string(reals[i]) == "-z.x" ||
					lower_string(reals[i]) == "-z.y" ||
					lower_string(reals[i]) == "-z.z" ||
					lower_string(reals[i]) == "-z.w" )
				{
					found_var = true;
				}
			}
		}
	}

	return found_var;
}


bool quaternion_julia_set_equation_parser::CleanupEquationTokens(vector<string> &equation)
{
	bool trimmed_something = false;

	bool trim = true; 

	// trim ( ) at begin/end
	while(equation.size() > 1 && true == trim)
	{
		if(equation[0] != "(" || equation[equation.size()-1] != ")")
		{
			trim = false;
			continue;
		}
		else
		{
			size_t paren_depth = 1;

			for(size_t i = 1; i < equation.size(); i++)
			{
				if(equation[i] == "(")
					paren_depth++;
				if(equation[i] == ")")
					paren_depth--;

				if(paren_depth == 0)
				{
					if(i != equation.size() - 1)
					{
						trim = false;
						break;
					}
					else
					{
						equation.erase(equation.end() - 1);
						equation.erase(equation.begin());
						trimmed_something = true;

						//for(size_t i = 0; i < equation.size(); i++)
						//cout << equation[i] << ' ';
						//cout << endl << endl;

					}
				}
			}
		}
	}

	// trim ( ) where there's no math operators inside and it's not following a function.
	trim = true;
	while(equation.size() > 2 && true == trim)
	{
		trim = false;

		for(size_t i = 0; i < equation.size() - 2 && trim == false; i++)
		{
			// if (C) or (Z)
			if( equation[i] == "(" && 
				(lower_string(equation[i+1]) == "c" || lower_string(equation[i+1]) == "z") &&
				equation[i+2] == ")" )
			{
				// if sine(C), etc, don't do anything
				if(i != 0 && is_function(equation[i-1]))
					continue;

				equation.erase(equation.begin() + i+2);
				equation.erase(equation.begin() + i);

				trim = true;
				trimmed_something = true;

				//for(size_t i = 0; i < equation.size(); i++)
				//cout << equation[i] << ' ';
				//cout << endl << endl;

				break;
			}
		}
	}

	// check if any C / Z .swizzles are next to each other, if so join them
	trim = true;
	while(equation.size() > 2 && true == trim)
	{
		trim = false;

		for(size_t i = 0; i < equation.size() - 1 && trim == false; i++)
		{
			if( (lower_string(equation[i]) == "c" || lower_string(equation[i]) == "z") &&
				is_raw_swizzle(equation[i+1]) )
			{
				equation[i] += equation[i+1];
				equation.erase(equation.begin() + i + 1);

				trim = true;
				trimmed_something = true;

				//for(size_t i = 0; i < equation.size(); i++)
				//cout << equation[i] << ' ';
				//cout << endl << endl;

				break;
			}
		}
	}

	// trim ( ) where there's no math operators inside and it's not following a function and not followed by a raw swizzle
	trim = true;
	while(equation.size() > 2 && true == trim)
	{
		trim = false;

		for(size_t i = 0; i < equation.size() - 2 && trim == false; i++)
		{
			// if (C) or (Z)
			if( equation[i] == "(" && 
				(is_variable_swizzle(equation[i+1]) || is_constant_swizzle(equation[i+1])) &&
				equation[i+2] == ")" )
			{
				// if sine(C), etc, don't do anything
				if(i != 0 && is_function(equation[i-1]))
					continue;

				if(i != equation.size() - 3 && is_raw_swizzle(equation[i+3]))
					continue;

				equation.erase(equation.begin() + i+2);
				equation.erase(equation.begin() + i);

				trim = true;
				trimmed_something = true;

				//for(size_t i = 0; i < equation.size(); i++)
				//cout << equation[i] << ' ';
				//cout << endl << endl;

				break;
			}
		}
	}




	//trim (( )) in equation
	trim = true;
	while(equation.size() > 4 && true == trim)
	{
		trim = false;

		//		0 1 2 3 4
		//		( ( C ) )

		for(size_t i = 0; i < equation.size() - 4 && trim == false; i++)
		{
			if(equation[i] == "(" && equation[i+1] == "(")
			{
				for(size_t j = i + 4; j < equation.size() && trim == false; j++)
				{
					if(equation[j-1] == ")" && equation[j] == ")")
					{
						size_t opening_parenthesis_count = 0;
						size_t closing_parenthesis_count = 0;

						bool found_closing_too_soon = false;

						// check for mismatched parentheses
						for(size_t k = i+2; k < j-1; k++)
						{
							if(equation[k] == "(")
								opening_parenthesis_count++;
							else if(equation[k] == ")")
								closing_parenthesis_count++;

							if(closing_parenthesis_count > opening_parenthesis_count)
							{
								found_closing_too_soon = true;
								break;
							}
						}

						if(false == found_closing_too_soon && opening_parenthesis_count == closing_parenthesis_count)
						{
							equation.erase(equation.begin() + j);
							equation.erase(equation.begin() + i);

							trim = true;
							trimmed_something = true;

							//for(size_t i = 0; i < equation.size(); i++)
							//cout << equation[i] << ' ';
							//cout << endl << endl;
						}
					}
				}
			}
		}
	}


	// trim unnecessary parentheses around functions
	trim = true;
	while(equation.size() > 4 && true == trim)
	{
		trim = false;

		//		0 1 2 3 4
		//		( ( C ) )
		//	    ( sin [ X ] )
		//		0  1  2 3 4 5
		for(size_t i = 0; i < equation.size() - 5 && trim == false; i++)
		{
			if(equation[i] == "(" && is_function(equation[i+1]))
			{
				for(size_t j = i + 4; j < equation.size() - 1 && trim == false; j++)
				{
					if(equation[j] == "]")
					{
						if(equation[j+1] == ")")
						{
							equation.erase(equation.begin() + j+1);
							equation.erase(equation.begin() + i);

							trim = true;
							trimmed_something = true;
						}
						else
						{
							break;
						}
					}
				}
			}
		}
	}






	// expand swizzles
	for(size_t i = 0; i < equation.size(); i++)
	{
		if(is_raw_swizzle(equation[i]))
		{
			if(equation[i].size() < 5)
			{
				char last = equation[i][equation[i].size() - 1];

				for(size_t j = 0; j < 5 - equation[i].size(); j++)
				{
					equation[i] += last;
				}
			}
		}
		else if(is_variable_swizzle(equation[i]) || is_constant_swizzle(equation[i]))
		{
			if(equation[i].size() < 6)
			{
				char last = equation[i][equation[i].size() - 1];

				for(size_t j = 0; j < 6 - equation[i].size(); j++)
				{
					equation[i] += last;
				}
			}
		}
	}



	// merge adjacent swizzles..
	// ie: (C.xyyz).yzwx == C.yyzx
	trim = true;
	while(equation.size() > 4 && true == trim)
	{
		trim = false;

		//	    ( C.xyzw ) .xyz
		//		0    1   2   3
		for(size_t i = 0; i < equation.size() - 3 && trim == false; i++)
		{
			if( equation[i] == "(" &&
				(is_variable_swizzle(equation[i+1]) || is_constant_swizzle(equation[i+1]) ) &&
				equation[i+2] == ")" &&
				is_raw_swizzle(equation[i+3]) )
			{
				string new_swizzle = ".";

				for(size_t j = 0; j < 4; j++)
				{
					char temp_char = tolower(equation[i+3][j+1]);

					if(temp_char == 'x')
						new_swizzle += equation[i+1][2];
					else if(temp_char == 'y')
						new_swizzle += equation[i+1][3];
					else if(temp_char == 'z')
						new_swizzle += equation[i+1][4];
					else if(temp_char == 'w')
						new_swizzle += equation[i+1][5];
				}

				equation[i+1] = equation[i+1][0] + new_swizzle;

				equation.erase(equation.begin() + i+3);
				equation.erase(equation.begin() + i+2);
				equation.erase(equation.begin() + i);

				trim = true;
				trimmed_something = true;
			}
		}
	}


	return trimmed_something;
}




void quaternion_julia_set_equation_parser::GetTerms(vector<string> src_equation, vector<term> &ordered_terms)
{
	vector<term> temp_terms;

	size_t level = 0;
	size_t max_level = 0;

	term temp_term;
	temp_term.level = level;



	// split into different levels
	for(vector<string>::const_iterator i = src_equation.begin(); i != src_equation.end(); i++)
	{
		if(*i == "(")
		{
			temp_term.text.push_back("NEXT");
			temp_terms.push_back(temp_term);

			temp_term.level = ++level;

			if(level > max_level)
				max_level = level;

			temp_term.text.resize(0);
			temp_term.text.push_back("BEGIN");
		}
		else if(*i == ")")
		{
			temp_term.text.push_back("END");
			temp_terms.push_back(temp_term);

			temp_term.level = --level;
			temp_term.text.resize(0);
		}
		else
		{
			if(lower_string(*i) != "quat")
				temp_term.text.push_back(*i);
		}
	}

	if(temp_term.text.size() != 0)
		temp_terms.push_back(temp_term);


	vector<term> final_terms;
	final_terms.resize(max_level + 1);

	for(size_t i = 0; i < temp_terms.size(); i++)
	{
		for(size_t j = 0; j < temp_terms[i].text.size(); j++)
		{
			final_terms[temp_terms[i].level].level = temp_terms[i].level;
			final_terms[temp_terms[i].level].text.push_back(temp_terms[i].text[j]);
		}
	}

	final_terms[0].text.insert(final_terms[0].text.begin(), "BEGIN");
	final_terms[0].text.push_back("END");


	vector< vector <term> > term_tree;
	term_tree.resize(max_level + 1);

	for(vector<term>::const_iterator i = final_terms.begin(); i != final_terms.end(); i++)
	{
		term temp_term;

		for(size_t j = 0; j < i->text.size(); j++)
		{
			if(i->text[j] == "END")
			{
				temp_term.level = i->level;
				term_tree[i->level].push_back(temp_term);
				temp_term.text.clear();
			}
			else
			{
				if(i->text[j] != "BEGIN")
				{
					temp_term.text.push_back(i->text[j]);
				}
			}
		}
	}

	for(size_t i = 0; i < term_tree.size(); i++)
	{
		size_t index = term_tree.size() - 1 - i;

		for(size_t j = 0; j < term_tree[index].size(); j++)
		{
			bool variable = false;

			for(size_t k = 0; k < term_tree[index][j].text.size(); k++)
			{
				if(is_variable(term_tree[index][j].text[k]) ||
					is_variable_swizzle(term_tree[index][j].text[k]) ||
					is_variable_quaternion(term_tree[index][j].text[k]) ||
					(term_tree[index][j].text[k].length() != 0 && term_tree[index][j].text[k][0] == 'V') )
				{
					variable = true;
					break;
				}
			}

			ordered_terms.push_back(term_tree[index][j]);
			ordered_terms[ordered_terms.size() - 1].constant = !variable;

			if(ordered_terms[ordered_terms.size() - 1].text.size() == 1 && (is_variable_quaternion(ordered_terms[ordered_terms.size() - 1].text[0]) || is_constant_quaternion(ordered_terms[ordered_terms.size() - 1].text[0])))
				ordered_terms[ordered_terms.size() - 1].quaternion = true;

			if(index != 0)
			{
				bool done = false;

				for(size_t k = 0; k < term_tree[index-1].size(); k++)
				{
					for(size_t l = 0; l < term_tree[index-1][k].text.size(); l++)
					{
						if(term_tree[index-1][k].text[l] == "NEXT")
						{
							ostringstream out;
							out << static_cast<long unsigned int>(ordered_terms.size() - 1);

							if(variable)
								term_tree[index-1][k].text[l] = "V#";
							else
								term_tree[index-1][k].text[l] = "C#";

							term_tree[index-1][k].text[l] += out.str();

							done = true;
						}

						if(done)
							break;
					}

					if(done)
						break;
				}
			}
		}
	}
}







bool quaternion_julia_set_equation_parser::Setup(const string &src_formula, string &error_output, const quaternion_math_base<double> *src_q_math, const quaternion_t<double> &src_C, const double &src_a)
{
	Cleanup(); // in this case, reset everything just in case this isn't the first time through Setup


	error_output = "";

	if(0 == src_q_math)
	{
		error_output = "NULL math library pointer.";
		return false;
	}

	q_math = (quaternion_math_base<double> *)src_q_math;
	C = src_C;
	a = src_a;


	// if custom_iterator wasn't the formula type specified
	// in the params passed into the quaternion_julia_set constructor, abort

	//	if(this->internal_params.formula != CUSTOM_ITERATOR)
	//	{
	//		error_output = "Internal quaternion_julia_set error: custom iterator not specified as formula type.";
	//		return false;
	//	}

	string formula = lower_string(src_formula);


	// get rid of any white space in the formula
	for(string::iterator i = formula.begin(); i != formula.end();)
	{
		if(isspace(*i))
			formula.erase(i);
		else
			i++;
	}


	// find colon, delete up to and including it, this allows the user to put a comment at the beginning of the formula
	size_t pos = formula.find_first_of(':');

	if(pos != string::npos)
		formula.erase(0, pos + 1);


	// replace parentheses characters
	for(string::iterator i = formula.begin(); i != formula.end(); i++)
	{
		if(*i == '[' || *i == '{')
			*i = '(';
		else if(*i == ']' || *i == '}')
			*i = ')';
	}


	// trim unnecessary parentheses at beginning and end of formula (ie: "(Z = Z*Z + C)" -> "Z = Z*Z + C")
	//	while(formula.length() >= 2)
	//	{
	//		if(formula[0] == '(' && formula[formula.length() - 1] == ')')
	//			formula = formula.substr(1, formula.length() - 2);
	//		else
	//			break;
	//	}

	// find ()'s
	for(size_t i = 0; i < formula.length() - 1; i++)
	{
		if(formula[i] == '(' && formula[i+1] == ')')
		{
			error_output = "Unrecognized symbol: '()'";
			return false;
		}
	}


	size_t opening_parenthesis_count = 0;
	size_t closing_parenthesis_count = 0;

	// check for mismatched parentheses
	for(string::iterator i = formula.begin(); i != formula.end(); i++)
	{
		if(*i == '(')
			opening_parenthesis_count++;
		else if(*i == ')')
			closing_parenthesis_count++;

		if(closing_parenthesis_count > opening_parenthesis_count)
		{
			error_output = "Unexpected symbol: ')'";
			return false;
		}
	}


	if(opening_parenthesis_count != closing_parenthesis_count)
	{
		if(opening_parenthesis_count > closing_parenthesis_count)
			error_output = "Unexpected symbol: '('";
		else
			error_output = "Unexpected symbol: ')'";

		return false;
	}


	string destination;

	pos = formula.find_first_of('=');

	if(pos != string::npos)
	{
		destination = formula.substr(0, pos);
		formula.erase(0, pos + 1);
	}

	destination = lower_string(destination);

	if(destination == "")
	{
		destination = "z";
	}
	else if(!is_valid_destination(destination))
	{
		error_output = "Unrecognized destination symbol: " + destination;
		return false;
	}

	// if Z= is the first part of the formula, erase it since it's assumed
	//	if(formula.length() >= 2)
	//		if((formula[0] == 'Z' || formula[0] == 'z') && formula[1] == '=')
	//			formula.erase(0, 2);


	// trim unnecessary parentheses at beginning and end of formula (ie: "(Z = Z*Z + C)" -> "Z = Z*Z + C")
	//	while(formula.length() >= 2)
	//	{
	//		if(formula[0] == '(' && formula[formula.length() - 1] == ')')
	//			formula = formula.substr(1, formula.length() - 2);
	//		else
	//			break;
	//	}


	// if the formula is (now) blank, abort
	if(formula == "")
	{
		error_output = "Custom formula text is blank.";
		return false;
	}


	// break up string
	vector<string> equation;
	equation.push_back("");

	// alternative quat processing code
	//bool processing_quat = false;

	for(size_t i = 0; i < formula.size(); i++)
	{
		if( formula[i] == '^' ||
			formula[i] == '*' ||
			formula[i] == '/' ||
			formula[i] == '+' ||
			formula[i] == '-' ||
			formula[i] == '(' ||
			formula[i] == ')'  )
		{
			if(equation.size() > 1 && equation[equation.size() - 2] == "^")
			{
				// before this if statement was added, things like Z ^ a - quat(2) would fail
				if(formula[i] == '-' || formula[i] == '.' || tolower(formula[i]) == 'e' || isdigit(formula[i]))
				{
					if(formula[i] == '-' && i != formula.size() - 1 && (formula[i+1] == '.' || tolower(formula[i+1]) == 'e' || isdigit(formula[i+1])))
					{
						// no need to perform any operations on the equation vector yet
						equation[equation.size() - 1] += formula[i];

						// it took me way too long to figure out where to put this statement :)
						continue;
					}
				}
			}

			if(formula[i] != ')')
			{

				// if it's possible that we may be still processing a quaternion constant
				if(equation.size() > 2 && lower_string(equation[equation.size() - 3]) == "quat" && equation[equation.size() - 2] == "(")
				{
					// no need to perform any operations on the equation vector yet
					equation[equation.size() - 1] += formula[i];

					// it took me way too long to figure out where to put this statement :)
					continue;
				}
			}

			if(equation[equation.size() - 1] != "")// && !processing_quat)
				equation.push_back("");

			equation[equation.size() - 1] += formula[i];

			if(i < formula.size() - 1)// && !processing_quat)
				equation.push_back("");
		}
		else
		{
			equation[equation.size() - 1] += formula[i];
		}

	}



	// check to see that only real numbers and a follow the ^ operator
	for(size_t i = 0; i < equation.size(); i++)
	{
		if(equation[i] == "^" && i != equation.size() - 1)
		{
			if(!is_real_number(equation[i+1]) && lower_string(equation[i+1]) != "a" && lower_string(equation[i+1]) != "quat")
			{
				error_output = "Unexpected symbol '" + equation[i+1] + "' following symbol: " + equation[i];
				return false;
			}
		}
	}

	// make pow exponents into quats
	for(size_t i = 0; i < equation.size(); i++)
	{
		if(equation[i] == "^" && i != equation.size() - 1 && lower_string(equation[i+1]) != "quat")
		{
			equation.insert(equation.begin() + i + 2, ")");
			equation.insert(equation.begin() + i + 1, "(");
			equation.insert(equation.begin() + i + 1, "quat");
			i = 0;
			continue;
		}
	}



	// expand quaternion parameters so that quat(Z.x) does not get interpreted as a plain variable later
	for(size_t i = 2; i < equation.size(); i++)
	{
		if(lower_string(equation[i-2]) == "quat")
		{
			size_t comma_count = 0;

			for(size_t j = 0; j < equation[i].size(); j++)
			{
				if(equation[i][j] == ',')
					comma_count++;

				if(comma_count == 3)
					break;
			}

			comma_count = 3 - comma_count;

			for(size_t j = 0; j < comma_count; j++)
				equation[i] += ",0";
		}
	}


	for(size_t i = 0; i < equation.size(); i++)
	{
		if(i > 1 && equation[i-1] == "^" && is_real_number(equation[i]))
		{
			continue;
		}
		else if(is_math_operator(equation[i]))
		{
			// do tests to see that ( - Z + C + ) fails

			if(i == 0)// || is_math_operator(equation[i-1]) || i == equation.size() - 1)
			{
				error_output = "Unexpected symbol at beginning of formula: " + equation[i];
				return false;
			}
			else if(is_math_operator(equation[i-1]))
			{
				error_output = "Unexpected symbol '" + equation[i] + "' following symbol: " + equation[i-1];
				return false;
			}
			else if(equation[i-1] == "(")
			{
				error_output = "Unexpected symbol '" + equation[i] + "' following symbol: " + equation[i-1];
				return false;
			}
			else if(i == equation.size() - 1)
			{
				error_output = "Unexpected symbol at end of formula: " + equation[i];
				return false;
			}

			continue;
		}
		else if(is_variable(equation[i]) || is_variable_swizzle(equation[i]) || is_constant(equation[i]) || is_constant_swizzle(equation[i]))
		{
			if(lower_string(equation[i]) == "a")
			{
				if(i == 0)
				{
					error_output = "Unexpected symbol at beginning of formula: " + equation[i] + "\nUse quat() function to mix reals with quaternions.";
					return false;
				}
				else if(equation[i-1] != "^")
				{
					error_output = "Unexpected symbol '" + equation[i] + "' following symbol: " + equation[i-1] + "\nUse quat() function to mix reals with quaternions.";
					return false;
				}
			}

			continue;
		}
		else if(equation[i] == "(" || equation[i] == ")")
		{
			if(equation[i] == "(" && i != 0 && !is_math_operator(equation[i-1]) && !is_function(equation[i-1]) && equation[i-1] != "(")
			{
				error_output  = "Unexpected symbol '" + equation[i] + "' following symbol: " + equation[i-1];
				return false;
			}
			else if(equation[i] == ")" && i != 0 && is_math_operator(equation[i-1]))
			{
				error_output = "Unexpected symbol '" + equation[i] + "' following symbol: " + equation[i-1];
				return false;
			}

			continue;
		}
		else if(is_constant_quaternion(equation[i]))
		{
			string last_function_name = "";

			for(size_t j = 0; j < i; j++)
			{
				if(is_function(equation[j]))
					last_function_name = equation[j];
			}

			if(lower_string(last_function_name) == "quat")
				continue;
			else
			{
				error_output = "Unexpected symbol '" + equation[i] + "'";
				return false;
			}
		}
		else if(is_variable_quaternion(equation[i]))
		{
			string last_function_name = "";

			for(size_t j = 0; j < i; j++)
			{
				if(is_function(equation[j]))
					last_function_name = equation[j];
			}

			if(lower_string(last_function_name) == "quat")
				continue;
			else
			{
				error_output = "Unexpected symbol '" + equation[i] + "'";
				return false;
			}
		}
		else if(is_raw_swizzle(equation[i]) && i != 0 && equation[i-1] == ")")
		{
			continue;
		}
		else if(is_function(equation[i]))
		{
			if(i == equation.size() - 1)
			{
				error_output = "Unexpected function symbol: " + equation[i];
				return false;
			}
			else if(equation[i+1] != "(")
			{
				error_output = "Function symbol not followed by '(': " + equation[i];
				return false;
			}
			else if(i != 0)
			{
				if( equation[i-1] != "(" &&
					!is_math_operator(equation[i-1]) )
				{
					error_output = "Unexpected symbol '" + equation[i] + "' following symbol: " + equation[i-1];
					return false;
				}
			}

			continue;
		}
		else
		{
			if(equation[i][equation[i].size()-1] == '=')
				error_output = "Unexpected symbol: " + equation[i];
			else
				error_output = "Unrecognized symbol: " + equation[i];

			return false;
		}
	}




	// convert function ()'s to []'s
	if(equation.size() > 1)
	for(size_t i = 1; i < equation.size() - 2; i++)
	{
		if(equation[i] == "(" && is_function(equation[i-1]))
		{
			equation[i] = "[";

			size_t paren_depth = 0;

			for(size_t j = i+1; j < equation.size(); j++)
			{
				if(equation[j] == "(")
				{
					paren_depth++;
				}
				else if(equation[j] == ")")
				{
					if(paren_depth == 0)
					{
						equation[j] = "]";
						break;
					}
					else
					{
						paren_depth--;
					}
				}
			}
		}
	}



	// keep trimming, thought i have never seen any equation that needs more than one trim
	while(CleanupEquationTokens(equation));

	if(equation.size() == 0)
	{
		error_output = "Processed formula contains no tokens. This is a bug, although I'm sure you deliberately tried to break the tokenizer. Bad user, bad bad bad. :)";
		return false;
	}

	// convert function ()'s to []'s
	for(size_t i = 0; i < equation.size(); i++)
	{
		if(equation[i] == "[")
			equation[i] = "(";
		else if(equation[i] == "]")
			equation[i] = ")";
	}





	unique_formula_string = destination;
	unique_formula_string += "=";



	for(size_t i = 0; i < equation.size(); i++)
	{


		unique_formula_string += equation[i];

	}
//	cout << endl;



	vector<term> ordered_terms;
	GetTerms(equation, ordered_terms);




	for(size_t i = 0; i < ordered_terms.size(); i++)
	{
		if(ordered_terms[i].constant == false)
			cout << "V";
		else
			cout << "C";

		if(ordered_terms[i].quaternion == false)
			cout << "E";
		else
			cout << "Q";

		cout << static_cast<long unsigned int>(i) << ": ";

		for(size_t j = 0; j < ordered_terms[i].text.size(); j++)
			cout << ordered_terms[i].text[j] << " ";

		cout << endl;
	}

	if(false == compile_ordered_terms(ordered_terms))
	{
		error_output = "Internal compile error.";
		return false;
	}

	SetupFinalZCopy(destination);


	if(false == assemble_compiled_instructions())
	{
		error_output = "Internal assembly error.";
		return false;
	}

	return true;
}

bool quaternion_julia_set_equation_parser::compile_ordered_terms(const vector<term> &ordered_terms)
{
	try
	{
		answers.reserve(ordered_terms.size());
		answers.resize(ordered_terms.size());

		instructions.reserve(ordered_terms.size());
		instructions.resize(ordered_terms.size());

		scratch_heap.reserve(ordered_terms.size());
		scratch_heap.resize(ordered_terms.size());
	}
	catch(...)
	{
		return false;
	}

	// set up quaternions and single term equations
	for(size_t i = 0; i < ordered_terms.size(); i++)
	{
		if(ordered_terms[i].text.size() != 1)
			continue;

		bool is_last_term = false;

		if(i == ordered_terms.size() - 1)
			is_last_term = true;

		if(ordered_terms[i].quaternion == true)
		{
			vector<string> tokens = STLStrTok(",", ordered_terms[i].text[0]);

			if(tokens.size() != 4)
				return false;

			if(ordered_terms[i].constant == true)
			{
				SetupConstantQuaternion(tokens, i);
//				cout << ordered_terms[i].text[0] << endl;
//				cout << answers[i].x << " " << answers[i].y << " " << answers[i].z << " " << answers[i].w << endl;
//				cout << endl;
			}
			else
			{
				SetupVariableQuaternion(tokens, i);
//				cout << ordered_terms[i].text[0] << endl;
//				cout << answers[i].x << " " << answers[i].y << " " << answers[i].z << " " << answers[i].w << endl;
//				cout << scratch_heap[i][0].x << " " << scratch_heap[i][0].y << " " << scratch_heap[i][0].z << " " << scratch_heap[i][0].w << endl;
//				cout << endl;
			}
		}
		else
		{
			if(ordered_terms[i].constant == true)
			{
				SetupSingleConstantEquation(ordered_terms[i].text[0], i);
//				cout << answers[i].x << " " << answers[i].y << " " << answers[i].z << " " << answers[i].w << endl;
//				cout << endl;
			}
			else
			{
				SetupSingleVariableEquation(ordered_terms[i].text[0], i);
//				if(instructions[i][0].f == &quaternion_math_base<double>::swizzle)
//					cout << scratch_heap[i][0].x << " " << scratch_heap[i][0].y << " " << scratch_heap[i][0].z << " " << scratch_heap[i][0].w << endl;
//				else
//					cout << "copy" << endl;
//
//				cout << endl;
			}
		}
	}




	// setup multipart terms (quaternions and single part terms are done in the loop above)
	for(size_t i = 0; i < ordered_terms.size(); i++)
	{
		if(ordered_terms[i].text.size() == 1)
			continue;

		if(ordered_terms[i].constant == true)
		{
			SetupConstantEquation(ordered_terms[i].text, i);
//			cout << ordered_terms[i].text[0] << endl;
//			cout << answers[i].x << " " << answers[i].y << " " << answers[i].z << " " << answers[i].w << endl;
//			cout << endl;

		}
		else
		{
			SetupVariableEquation(ordered_terms[i].text, i);
//			cout << ordered_terms[i].text[0] << endl;
//			cout << answers[i].x << " " << answers[i].y << " " << answers[i].z << " " << answers[i].w << endl;
//			cout << scratch_heap[i][0].x << " " << scratch_heap[i][0].y << " " << scratch_heap[i][0].z << " " << scratch_heap[i][0].w << endl;
//			cout << endl;
		}
	}





	return true;
}

