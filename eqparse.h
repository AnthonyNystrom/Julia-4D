#ifndef EQPARSE_H
#define EQPARSE_H



#include "quaternion_math.h"


#include "utilities.h"

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <sstream>
using std::istringstream;


class term
{
public:
	size_t level;
	vector<string> text;
	bool constant;
	bool quaternion;
};




typedef void (quaternion_math_base<double>::*qmath_func_ptr)(const quaternion_t<double> *const, const quaternion_t<double> *const, quaternion_t<double> *const);

#define TOKENIZED_INSTRUCTION_DEST_ANSWER 0
#define TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP 1
#define TOKENIZED_INSTRUCTION_DEST_CONSTANTS_SCRATCH_HEAP 2
#define TOKENIZED_INSTRUCTION_DEST_Z 3
#define TOKENIZED_INSTRUCTION_DEST_C 4
#define TOKENIZED_INSTRUCTION_DEST_NULL 5

class tokenized_instruction
{
public:

	tokenized_instruction()
	{
		a_type = TOKENIZED_INSTRUCTION_DEST_NULL;
		b_type = TOKENIZED_INSTRUCTION_DEST_NULL;
		out_type = TOKENIZED_INSTRUCTION_DEST_NULL;
		f = 0;
	}

	qmath_func_ptr f;
	size_t a_type, b_type, out_type;
	size_t a_index, b_index, out_index;
};

class assembled_instruction
{
public:
	qmath_func_ptr f;
	quaternion_t<double> *a, *b, *out;
};

class function_mapping
{
public:
	function_mapping(const string &src_function_name, qmath_func_ptr src_f)
	{
		function_name = lower_string(src_function_name);
		f = src_f;
	}

	string function_name;
	qmath_func_ptr f;
};

#include <iostream>
using namespace std;


class quaternion_julia_set_equation_parser
{
public:

	quaternion_julia_set_equation_parser()
	{
		execution_stack = 0;
		SetupFunctionMap();
	}

	~quaternion_julia_set_equation_parser()
	{
		Cleanup();
	}

	void SetupFunctionMap(void);
	qmath_func_ptr GetFunctionInstruction(const string &src_token);

	void Cleanup(void)
	{
		unique_formula_string = "";
		answers.clear();
		constant_scratch_heap.clear();
		instructions.clear();
		scratch_heap.clear();

		if(0 != execution_stack)
			delete [] execution_stack;

		execution_stack_size = 0;
	}

	// lala: must do setup of variable / answer arrays, also precalcing any constant Q or Es
	bool Setup(const string &src_formula, string &error_output, const quaternion_math_base<double> *src_q_math, const quaternion_t<double> &src_C, const double &src_a);

	// don't need c or a or qmath, those are passed in through setup
	double Iterate(const quaternion_t<double> &src_Z, const long unsigned int &max_iterations, const double &infinite_length)
	{
		double length = 0.0;
		Z = src_Z;

		for(long unsigned int i = 0; i < max_iterations; i++)
		{
			//cout << Z.x << " " << Z.y << " " << Z.z << " " << Z.w << endl;
			// execute one pass
			Execute();
			//cout << answers[1].x << " " << answers[1].y << " " << answers[1].z << " " << answers[1].w << endl;

			//cout << Z.x << " " << Z.y << " " << Z.z << " " << Z.w << endl;

			if((length = (Z.x*Z.x + Z.y*Z.y + Z.z*Z.z + Z.w*Z.w)) >= infinite_length*infinite_length)
				break;
		}

		return sqrt(length);
	}

	string GetUniqueFormulaString(void)
	{
		return unique_formula_string;
	}

protected:

	void Execute(void)
	{
		for(size_t i = 0; i < execution_stack_size; i++)
			(q_math->*execution_stack[i].f)(execution_stack[i].a, execution_stack[i].b, execution_stack[i].out);
	}

	void SetupConstantQuaternion(const vector<string> &tokens, const size_t &ordered_term_index)
	{
		for(size_t i = 0; i < 4; i++)
		{
			double *dest_double = 0;

			if(i == 0)
				dest_double = &answers[ordered_term_index].x;
			if(i == 1)
				dest_double = &answers[ordered_term_index].y;
			if(i == 2)
				dest_double = &answers[ordered_term_index].z;
			if(i == 3)
				dest_double = &answers[ordered_term_index].w;

			string token = lower_string(tokens[i]);

			if(token == "c.x")
				*dest_double = C.x;
			else if(token == "-c.x")
				*dest_double = -C.x;
			else if(token == "c.y")
				*dest_double = C.y;
			else if(token == "-c.y")
				*dest_double = -C.y;
			else if(token == "c.z")
				*dest_double = C.z;
			else if(token == "-c.z")
				*dest_double = -C.z;
			else if(token == "c.w")
				*dest_double = C.w;
			else if(token == "-c.w")
				*dest_double = -C.w;
			else if(token == "a")
				*dest_double = a;
			else if(token == "-a")
				*dest_double = -a;
			else
			{
				istringstream in(token);
				in >> *dest_double;
			}
		}
	}

	void SetupVariableQuaternion(const vector<string> &tokens, const size_t &ordered_term_index)
	{
		quaternion_t<double> copymask; // defaults to zeroes

		for(size_t i = 0; i < 4; i++)
		{
			double *dest_double = 0;
			double *copy_mask_dest_double = 0;

			if(i == 0)
			{
				dest_double = &answers[ordered_term_index].x;
				copy_mask_dest_double = &copymask.x;
			}
			if(i == 1)
			{
				dest_double = &answers[ordered_term_index].y;
				copy_mask_dest_double = &copymask.y;
			}
			if(i == 2)
			{
				dest_double = &answers[ordered_term_index].z;
				copy_mask_dest_double = &copymask.z;
			}
			if(i == 3)
			{
				dest_double = &answers[ordered_term_index].w;
				copy_mask_dest_double = &copymask.w;
			}

			string token = lower_string(tokens[i]);

			if(token == "z.x")
				*copy_mask_dest_double = 1.0;
			else if(token == "-z.x")
				*copy_mask_dest_double = -1.0;
			else if(token == "z.y")
				*copy_mask_dest_double = 2.0;
			else if(token == "-z.y")
				*copy_mask_dest_double = -2.0;
			else if(token == "z.z")
				*copy_mask_dest_double = 3.0;
			else if(token == "-z.z")
				*copy_mask_dest_double = -3.0;
			else if(token == "z.w")
				*copy_mask_dest_double = 4.0;
			else if(token == "-z.w")
				*copy_mask_dest_double = -4.0;
			else if(token == "c.x")
				*dest_double = C.x;
			else if(token == "-c.x")
				*dest_double = -C.x;
			else if(token == "c.y")
				*dest_double = C.y;
			else if(token == "-c.y")
				*dest_double = -C.y;
			else if(token == "c.z")
				*dest_double = C.z;
			else if(token == "-c.z")
				*dest_double = -C.z;
			else if(token == "c.w")
				*dest_double = C.w;
			else if(token == "-c.w")
				*dest_double = -C.w;
			else if(token == "a")
				*dest_double = a;
			else if(token == "-a")
				*dest_double = -a;
			else
			{
				istringstream in(token);
				in >> *dest_double;
			}
		}

		// add copy mask to scratch heap
		scratch_heap[ordered_term_index].push_back(copymask);

		// setup copy_masked instruction to use Z as a, 0th scratch heap item as b, and this equation level's answer as out
		tokenized_instruction instruction;

		instruction.f = &quaternion_math_base<double>::copy_masked;
		instruction.a_type = TOKENIZED_INSTRUCTION_DEST_Z;
		instruction.b_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
		instruction.b_index = 0;
		instruction.out_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
		instruction.out_index = ordered_term_index;

		instructions[ordered_term_index].push_back(instruction);
	}

	void SetupSingleConstantEquation(const string &src_token, const size_t &ordered_term_index)
	{
		string token = lower_string(src_token);

		answers[ordered_term_index] = C;

		if(is_constant_swizzle(token)) // assumes that swizzle has been expanded
		{
			quaternion_t<double> swizzlemask;

			for(size_t i = 0; i < 4; i++)
			{
				double *swizzle_mask_dest_double = 0;

				if(i == 0)
					swizzle_mask_dest_double = &swizzlemask.x;
				if(i == 1)
					swizzle_mask_dest_double = &swizzlemask.y;
				if(i == 2)
					swizzle_mask_dest_double = &swizzlemask.z;
				if(i == 3)
					swizzle_mask_dest_double = &swizzlemask.w;

				char char_token = token[i + 2];

				if(char_token == 'x')
					*swizzle_mask_dest_double = 1.0;
				else if(char_token == 'y')
					*swizzle_mask_dest_double = 2.0;
				else if(char_token == 'z')
					*swizzle_mask_dest_double = 3.0;
				else if(char_token == 'w')
					*swizzle_mask_dest_double = 4.0;
			}

			q_math->swizzle(&answers[ordered_term_index], &swizzlemask, &answers[ordered_term_index]);
		}
	}

	void SetupSingleVariableEquation(const string &src_token, const size_t &ordered_term_index)
	{
		string token = lower_string(src_token);

		if(is_variable(token)) // 'z', so do copy
		{
			tokenized_instruction instruction;

			instruction.f = &quaternion_math_base<double>::copy;
			instruction.a_type = TOKENIZED_INSTRUCTION_DEST_Z;
			instruction.b_type = TOKENIZED_INSTRUCTION_DEST_NULL;
			instruction.out_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
			instruction.out_index = ordered_term_index;

			instructions[ordered_term_index].push_back(instruction);
		}
		else if(is_variable_swizzle(token)) // z.????, so do swizzle instruction
		{
			quaternion_t<double> swizzlemask;

			for(size_t i = 0; i < 4; i++)
			{
				double *swizzle_mask_dest_double = 0;

				if(i == 0)
					swizzle_mask_dest_double = &swizzlemask.x;
				if(i == 1)
					swizzle_mask_dest_double = &swizzlemask.y;
				if(i == 2)
					swizzle_mask_dest_double = &swizzlemask.z;
				if(i == 3)
					swizzle_mask_dest_double = &swizzlemask.w;

				char char_token = token[i + 2];

				if(char_token == 'x')
					*swizzle_mask_dest_double = 1.0;
				else if(char_token == 'y')
					*swizzle_mask_dest_double = 2.0;
				else if(char_token == 'z')
					*swizzle_mask_dest_double = 3.0;
				else if(char_token == 'w')
					*swizzle_mask_dest_double = 4.0;
			}

			// add swizzle mask to scratch heap
			scratch_heap[ordered_term_index].push_back(swizzlemask);

			// setup swizzle instruction to use Z as a, 0th scratch heap item as b, and this equation level's answer as out
			tokenized_instruction instruction;

			instruction.f = &quaternion_math_base<double>::swizzle;
			instruction.a_type = TOKENIZED_INSTRUCTION_DEST_Z;
			instruction.b_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
			instruction.b_index = 0;
			instruction.out_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
			instruction.out_index = ordered_term_index;

			instructions[ordered_term_index].push_back(instruction);
		}
	}


	void SetupConstantEquation(vector<string> tokens, const size_t &ordered_term_index)
	{
		// do non-raw swizzles
		for(size_t i = 0; i < tokens.size(); i++)
		{
			string token = lower_string(tokens[i]);

			if(!is_constant_swizzle(token))
				continue;

			// get next scratch heap address
			size_t next_scratch_heap_index = scratch_heap[ordered_term_index].size();
			scratch_heap[ordered_term_index].push_back(C);

			quaternion_t<double> swizzlemask;

			for(size_t j = 0; j < 4; j++)
			{
				double *swizzle_mask_dest_double = 0;

				if(j == 0)
					swizzle_mask_dest_double = &swizzlemask.x;
				if(j == 1)
					swizzle_mask_dest_double = &swizzlemask.y;
				if(j == 2)
					swizzle_mask_dest_double = &swizzlemask.z;
				if(j == 3)
					swizzle_mask_dest_double = &swizzlemask.w;

				char char_token = token[j + 2];

				if(char_token == 'x')
					*swizzle_mask_dest_double = 1.0;
				else if(char_token == 'y')
					*swizzle_mask_dest_double = 2.0;
				else if(char_token == 'z')
					*swizzle_mask_dest_double = 3.0;
				else if(char_token == 'w')
					*swizzle_mask_dest_double = 4.0;
			}

			q_math->swizzle(&scratch_heap[ordered_term_index][next_scratch_heap_index], &swizzlemask, &scratch_heap[ordered_term_index][next_scratch_heap_index]);

			ostringstream out;
			out << static_cast<long unsigned int>(next_scratch_heap_index);
			tokens[i] = "SH#";
			tokens[i] += out.str();
		}

		// do functions
		for(size_t i = 0; i < tokens.size(); i++)
		{
			if(is_function(tokens[i]))
			{
				// get function address
				tokenized_instruction instruction;
				instruction.f = GetFunctionInstruction(tokens[i]);

				// trim C#?? to ??
				string source_address_token = tokens[i + 1].substr(2, tokens[i + 1].length() - 2);

				// setup address #
				istringstream in(source_address_token);
				size_t src_answer_address = 0;
				in >> src_answer_address;

				(q_math->*instruction.f)(&answers[src_answer_address], 0, &answers[src_answer_address]);

				tokens.erase(tokens.begin() + i);

				i = 0;
			}
		}

		// do raw swizzles
		for(size_t i = 0; i < tokens.size(); i++)
		{
			if(is_raw_swizzle(tokens[i]))
			{
				string token = lower_string(tokens[i]);

				quaternion_t<double> swizzlemask;

				for(size_t j = 0; j < 4; j++)
				{
					double *swizzle_mask_dest_double = 0;

					if(j == 0)
						swizzle_mask_dest_double = &swizzlemask.x;
					if(j == 1)
						swizzle_mask_dest_double = &swizzlemask.y;
					if(j == 2)
						swizzle_mask_dest_double = &swizzlemask.z;
					if(j == 3)
						swizzle_mask_dest_double = &swizzlemask.w;

					char char_token = token[j + 1];

					if(char_token == 'x')
						*swizzle_mask_dest_double = 1.0;
					else if(char_token == 'y')
						*swizzle_mask_dest_double = 2.0;
					else if(char_token == 'z')
						*swizzle_mask_dest_double = 3.0;
					else if(char_token == 'w')
						*swizzle_mask_dest_double = 4.0;
				}


				// trim C#?? to ??
				string source_address_token = tokens[i - 1].substr(2, tokens[i - 1].length() - 2);

				// setup address #
				istringstream in(source_address_token);
				size_t input_address = 0;
				in >>input_address;

				q_math->swizzle(&answers[input_address], &swizzlemask, &answers[input_address]);

				tokens.erase(tokens.begin() + i);

				i = 0;
			}
		}

		// do ^
		for(size_t i = 0; i < tokens.size(); i++)
		{
			if("^" == tokens[i])
			{
				quaternion_t<double> *a = 0;
				quaternion_t<double> *pow_mask = 0;
				quaternion_t<double> *output = 0;

				// get/set input/output
				if("c" == lower_string(tokens[i - 1]))
				{
					size_t next_scratch_heap_index = scratch_heap[ordered_term_index].size();
					scratch_heap[ordered_term_index].push_back(C);

					a = &scratch_heap[ordered_term_index][next_scratch_heap_index];
					output = &scratch_heap[ordered_term_index][next_scratch_heap_index];

					ostringstream out;
					out << static_cast<long unsigned int>(next_scratch_heap_index);
					tokens[i-1] = "SH#";
					tokens[i-1] += out.str();
				}
				else
				{
					vector<string> src_tokens = STLStrTok("#", tokens[i - 1]);

					istringstream in(src_tokens[1]);
					size_t address = 0;
					in >> address;

					if("C" == src_tokens[0]) // constant equation token
					{
						a = &answers[address];
						output = &answers[address];
					}
					else
					{
						a = &scratch_heap[ordered_term_index][address];
						output = &scratch_heap[ordered_term_index][address];
					}
				}

				vector<string> dest_tokens = STLStrTok("#", tokens[i + 1]);

				// trim C#?? to ??
				string source_address_token = tokens[i + 1].substr(2, tokens[i + 1].length() - 2);

				// setup address #
				istringstream in(source_address_token);
				size_t src_answer_address = 0;
				in >> src_answer_address;

				(q_math->pow_dynamic)(a, &answers[src_answer_address], output);

				tokens.erase(tokens.begin() + i + 1);
				tokens.erase(tokens.begin() + i);

				i = 0;
			}
		}

		// do * /
		for(size_t i = 0; i < tokens.size(); i++)
		{
			if("*" == tokens[i] || "/" == tokens[i])
			{
				quaternion_t<double> *a = 0;
				quaternion_t<double> *b = 0;
				quaternion_t<double> *output = 0;

				// get/set a/output
				if("c" == lower_string(tokens[i - 1]))
				{
					size_t next_scratch_heap_index = scratch_heap[ordered_term_index].size();
					scratch_heap[ordered_term_index].push_back(C);

					a = &scratch_heap[ordered_term_index][next_scratch_heap_index];
					output = &scratch_heap[ordered_term_index][next_scratch_heap_index];

					ostringstream out;
					out << static_cast<long unsigned int>(next_scratch_heap_index);
					tokens[i-1] = "SH#";
					tokens[i-1] += out.str();
				}
				else
				{
					vector<string> src_tokens = STLStrTok("#", tokens[i - 1]);

					istringstream in(src_tokens[1]);
					size_t address = 0;
					in >> address;

					if("C" == src_tokens[0]) // constant equation token
					{
						a = &answers[address];
						output = &answers[address];
					}
					else
					{
						a = &scratch_heap[ordered_term_index][address];
						output = &scratch_heap[ordered_term_index][address];
					}
				}

				// get/set b
				if("c" == lower_string(tokens[i + 1]))
				{
					b = &C;
				}
				else
				{
					vector<string> src_tokens = STLStrTok("#", tokens[i + 1]);

					istringstream in(src_tokens[1]);
					size_t address = 0;
					in >> address;

					if("C" == src_tokens[0]) // constant equation token
					{
						b = &answers[address];
					}
					else
					{
						b = &scratch_heap[ordered_term_index][address];
					}
				}

				if("*" == tokens[i])
					(q_math->mul)(a, b, output);
				else
					(q_math->div)(a, b, output);

				tokens.erase(tokens.begin() + i + 1);
				tokens.erase(tokens.begin() + i);

				i = 0;
			}
		}

		// do + -
		for(size_t i = 0; i < tokens.size(); i++)
		{
			if("+" == tokens[i] || "-" == tokens[i])
			{
				quaternion_t<double> *a = 0;
				quaternion_t<double> *b = 0;
				quaternion_t<double> *output = 0;


				// get/set a/output
				if("c" == lower_string(tokens[i - 1]))
				{
					size_t next_scratch_heap_index = scratch_heap[ordered_term_index].size();
					scratch_heap[ordered_term_index].push_back(C);

					a = &scratch_heap[ordered_term_index][next_scratch_heap_index];
					output = &scratch_heap[ordered_term_index][next_scratch_heap_index];

					ostringstream out;
					out << static_cast<long unsigned int>(next_scratch_heap_index);
					tokens[i-1] = "SH#";
					tokens[i-1] += out.str();
				}
				else
				{
					vector<string> src_tokens = STLStrTok("#", tokens[i - 1]);

					istringstream in(src_tokens[1]);
					size_t address = 0;
					in >> address;

					if("C" == src_tokens[0]) // constant equation token
					{
						a = &answers[address];
						output = &answers[address];
					}
					else
					{
						a = &scratch_heap[ordered_term_index][address];
						output = &scratch_heap[ordered_term_index][address];
					}
				}

				// get/set b
				if("c" == lower_string(tokens[i + 1]))
				{
					b = &C;
				}
				else
				{
					vector<string> src_tokens = STLStrTok("#", tokens[i + 1]);

					istringstream in(src_tokens[1]);
					size_t address = 0;
					in >> address;

					if("C" == src_tokens[0]) // constant equation token
					{
						b = &answers[address];
					}
					else
					{
						b = &scratch_heap[ordered_term_index][address];
					}
				}

				if("+" == tokens[i])
					(q_math->add)(a, b, output);
				else
					(q_math->sub)(a, b, output);

				tokens.erase(tokens.begin() + i + 1);
				tokens.erase(tokens.begin() + i);

				i = 0;
			}
		}

		// copy to answer index
		quaternion_t<double> *a;

		// get/set a
		if("c" == lower_string(tokens[0]))
		{
			a = &C;
		}
		else
		{
			vector<string> src_tokens = STLStrTok("#", tokens[0]);

			istringstream in(src_tokens[1]);
			size_t address = 0;
			in >> address;

			if("C" == src_tokens[0]) // constant equation token
			{
				a = &answers[address];
			}
			else
			{
				a = &scratch_heap[ordered_term_index][address];
			}
		}

		(q_math->copy)(a, 0, &answers[ordered_term_index]);

		// don't need these anymore, this is a constant expression, no instructions will have been created
		scratch_heap[ordered_term_index].clear();
	}

	void SetupVariableEquation(vector<string> tokens, const size_t &ordered_term_index)
	{
		// do constant non-raw swizzles
		for(size_t i = 0; i < tokens.size(); i++)
		{
			string token = lower_string(tokens[i]);

			if(!is_constant_swizzle(token))
				continue;

			// get next constant scratch heap address
			size_t next_scratch_heap_index = constant_scratch_heap.size();
			constant_scratch_heap.push_back(C);

			quaternion_t<double> swizzlemask;

			for(size_t j = 0; j < 4; j++)
			{
				double *swizzle_mask_dest_double = 0;

				if(j == 0)
					swizzle_mask_dest_double = &swizzlemask.x;
				if(j == 1)
					swizzle_mask_dest_double = &swizzlemask.y;
				if(j == 2)
					swizzle_mask_dest_double = &swizzlemask.z;
				if(j == 3)
					swizzle_mask_dest_double = &swizzlemask.w;

				char char_token = token[j + 2];

				if(char_token == 'x')
					*swizzle_mask_dest_double = 1.0;
				else if(char_token == 'y')
					*swizzle_mask_dest_double = 2.0;
				else if(char_token == 'z')
					*swizzle_mask_dest_double = 3.0;
				else if(char_token == 'w')
					*swizzle_mask_dest_double = 4.0;
			}

			q_math->swizzle(&constant_scratch_heap[next_scratch_heap_index], &swizzlemask, &constant_scratch_heap[next_scratch_heap_index]);

			ostringstream out;
			out << static_cast<long unsigned int>(next_scratch_heap_index);
			tokens[i] = "CSH#";
			tokens[i] += out.str();
		}



		// do variable non-raw swizzles
		for(size_t i = 0; i < tokens.size(); i++)
		{
			string token = lower_string(tokens[i]);

			if(!is_variable_swizzle(token))
				continue;

			quaternion_t<double> swizzlemask;

			for(size_t j = 0; j < 4; j++)
			{
				double *swizzle_mask_dest_double = 0;

				if(j == 0)
					swizzle_mask_dest_double = &swizzlemask.x;
				if(j == 1)
					swizzle_mask_dest_double = &swizzlemask.y;
				if(j == 2)
					swizzle_mask_dest_double = &swizzlemask.z;
				if(j == 3)
					swizzle_mask_dest_double = &swizzlemask.w;

				char char_token = token[j + 2];

				if(char_token == 'x')
					*swizzle_mask_dest_double = 1.0;
				else if(char_token == 'y')
					*swizzle_mask_dest_double = 2.0;
				else if(char_token == 'z')
					*swizzle_mask_dest_double = 3.0;
				else if(char_token == 'w')
					*swizzle_mask_dest_double = 4.0;
			}

			tokenized_instruction instruction;
			instruction.f = &quaternion_math_base<double>::swizzle;

			instruction.a_type = TOKENIZED_INSTRUCTION_DEST_Z;

			instruction.b_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
			scratch_heap[ordered_term_index].push_back(swizzlemask);
			instruction.b_index = scratch_heap[ordered_term_index].size() - 1;

			instruction.out_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
			scratch_heap[ordered_term_index].push_back(quaternion_t<double>());
			instruction.out_index = scratch_heap[ordered_term_index].size() - 1;

			instructions[ordered_term_index].push_back(instruction);

			ostringstream out;
			out << static_cast<long unsigned int>(scratch_heap[ordered_term_index].size() - 1);
			tokens[i] = "SH#";
			tokens[i] += out.str();
		}


		// do functions
		for(size_t i = 0; i < tokens.size(); i++)
		{
			if(is_function(tokens[i]))
			{
				// get function address
				tokenized_instruction instruction;
				instruction.f = GetFunctionInstruction(tokens[i]);

				// can either be C# or V#
				string source_address_token = tokens[i + 1].substr(2, tokens[i + 1].length() - 2);

				// setup address #
				istringstream in(source_address_token);
				size_t src_answer_address = 0;
				in >> src_answer_address;

				if('C' == tokens[i+1][0])
				{
					(q_math->*instruction.f)(&answers[src_answer_address], 0, &answers[src_answer_address]);
				}
				else
				{
					instruction.a_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
					instruction.a_index = src_answer_address;
					instruction.b_type = TOKENIZED_INSTRUCTION_DEST_NULL;
					instruction.out_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
					scratch_heap[ordered_term_index].push_back(quaternion_t<double>());
					instruction.out_index = scratch_heap[ordered_term_index].size() - 1;

					instructions[ordered_term_index].push_back(instruction);

					ostringstream out;
					out << static_cast<long unsigned int>(scratch_heap[ordered_term_index].size() - 1);
					tokens[i+1] = "SH#";
					tokens[i+1] += out.str();
				}

				tokens.erase(tokens.begin() + i);

				i = 0;
			}
		}


		// do raw swizzles
		for(size_t i = 0; i < tokens.size(); i++)
		{
			if(is_raw_swizzle(tokens[i]))
			{
				string token = lower_string(tokens[i]);

				quaternion_t<double> swizzlemask;

				for(size_t j = 0; j < 4; j++)
				{
					double *swizzle_mask_dest_double = 0;

					if(j == 0)
						swizzle_mask_dest_double = &swizzlemask.x;
					if(j == 1)
						swizzle_mask_dest_double = &swizzlemask.y;
					if(j == 2)
						swizzle_mask_dest_double = &swizzlemask.z;
					if(j == 3)
						swizzle_mask_dest_double = &swizzlemask.w;

					char char_token = token[j + 1];

					if(char_token == 'x')
						*swizzle_mask_dest_double = 1.0;
					else if(char_token == 'y')
						*swizzle_mask_dest_double = 2.0;
					else if(char_token == 'z')
						*swizzle_mask_dest_double = 3.0;
					else if(char_token == 'w')
						*swizzle_mask_dest_double = 4.0;
				}


				// can either be C# or V# or SH#
				vector<string> src_tokens = STLStrTok("#", tokens[i - 1]);

				istringstream in(src_tokens[1]);
				size_t address = 0;
				in >> address;

				if("C" == src_tokens[0])
				{
					q_math->swizzle(&answers[address], &swizzlemask, &answers[address]);
				}
				else if("V" == src_tokens[0])
				{
					tokenized_instruction instruction;
					instruction.f = &quaternion_math_base<double>::swizzle;

					instruction.a_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
					instruction.a_index = address;

					instruction.b_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
					scratch_heap[ordered_term_index].push_back(swizzlemask);
					instruction.b_index = scratch_heap[ordered_term_index].size() - 1;

					instruction.out_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
					scratch_heap[ordered_term_index].push_back(quaternion_t<double>());
					instruction.out_index = scratch_heap[ordered_term_index].size() - 1;

					instructions[ordered_term_index].push_back(instruction);

					ostringstream out;
					out << static_cast<long unsigned int>(scratch_heap[ordered_term_index].size() - 1);
					tokens[i-1] = "SH#";
					tokens[i-1] += out.str();
				}
				else
				{
					tokenized_instruction instruction;
					instruction.f = &quaternion_math_base<double>::swizzle;

					instruction.a_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
					instruction.a_index = address;

					instruction.b_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
					scratch_heap[ordered_term_index].push_back(swizzlemask);
					instruction.b_index = scratch_heap[ordered_term_index].size() - 1;

					instruction.out_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
					instruction.out_index = address;

					instructions[ordered_term_index].push_back(instruction);
				}

				tokens.erase(tokens.begin() + i);

				i = 0;
			}
		}


		// do ^
		for(size_t i = 0; i < tokens.size(); i++)
		{
			if("^" == tokens[i])
			{
				tokenized_instruction instruction;
				instruction.f = &quaternion_math_base<double>::pow_dynamic;

				bool answer_requires_scratch_heap = true;

				if("c" == lower_string(tokens[i-1]))
				{
					instruction.a_type = TOKENIZED_INSTRUCTION_DEST_C;
				}
				else if("z" == lower_string(tokens[i-1]))
				{
					instruction.a_type = TOKENIZED_INSTRUCTION_DEST_Z;
				}
				else
				{
					vector<string> src_tokens = STLStrTok("#", tokens[i-1]);

					istringstream in(src_tokens[1]);
					size_t address = 0;
					in >> address;

					if("SH" == src_tokens[0])
					{
						instruction.a_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
						instruction.a_index = address;
						instruction.out_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
						instruction.out_index = address;

						answer_requires_scratch_heap = false;
					}
					else if("CSH" == src_tokens[0])
					{
						instruction.a_type = TOKENIZED_INSTRUCTION_DEST_CONSTANTS_SCRATCH_HEAP;
						instruction.a_index = address;
					}
					else if("C" == src_tokens[0])
					{
						instruction.a_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
						instruction.a_index = address;
					}
					else if("V" == src_tokens[0])
					{
						instruction.a_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
						instruction.a_index = address;
					}
				}

				if(true == answer_requires_scratch_heap)
				{
					instruction.out_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
					scratch_heap[ordered_term_index].push_back(quaternion_t<double>());
					instruction.out_index = scratch_heap[ordered_term_index].size() - 1;

					ostringstream out;
					out << static_cast<long unsigned int>(scratch_heap[ordered_term_index].size() - 1);
					tokens[i-1] = "SH#";
					tokens[i-1] += out.str();
				}

				// setup b
				if("c" == lower_string(tokens[i+1]))
				{
					instruction.b_type = TOKENIZED_INSTRUCTION_DEST_C;
				}
				else if("z" == lower_string(tokens[i+1]))
				{
					instruction.b_type = TOKENIZED_INSTRUCTION_DEST_Z;
				}
				else
				{
					vector<string> src_tokens = STLStrTok("#", tokens[i+1]);

					istringstream in(src_tokens[1]);
					size_t address = 0;
					in >> address;

					if("SH" == src_tokens[0])
					{
						instruction.b_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
						instruction.b_index = address;
					}
					else if("CSH" == src_tokens[0])
					{
						instruction.b_type = TOKENIZED_INSTRUCTION_DEST_CONSTANTS_SCRATCH_HEAP;
						instruction.b_index = address;
					}
					else if("C" == src_tokens[0])
					{
						instruction.b_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
						instruction.b_index = address;
					}
					else if("V" == src_tokens[0])
					{
						instruction.b_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
						instruction.b_index = address;
					}
				}

				instructions[ordered_term_index].push_back(instruction);

				tokens.erase(tokens.begin() + i + 1);
				tokens.erase(tokens.begin() + i);

				i = 0;
			}
		}



		// do * /
		for(size_t i = 0; i < tokens.size(); i++)
		{
			if("*" == tokens[i] || "/" == tokens[i])
			{
				tokenized_instruction instruction;

				if("*" == tokens[i])
					instruction.f = &quaternion_math_base<double>::mul;
				else
					instruction.f = &quaternion_math_base<double>::div;

				bool answer_requires_scratch_heap = true;

				if("c" == lower_string(tokens[i-1]))
				{
					instruction.a_type = TOKENIZED_INSTRUCTION_DEST_C;
				}
				else if("z" == lower_string(tokens[i-1]))
				{
					instruction.a_type = TOKENIZED_INSTRUCTION_DEST_Z;
				}
				else
				{
					vector<string> src_tokens = STLStrTok("#", tokens[i-1]);

					istringstream in(src_tokens[1]);
					size_t address = 0;
					in >> address;

					if("SH" == src_tokens[0])
					{
						instruction.a_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
						instruction.a_index = address;
						instruction.out_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
						instruction.out_index = address;

						answer_requires_scratch_heap = false;
					}
					else if("CSH" == src_tokens[0])
					{
						instruction.a_type = TOKENIZED_INSTRUCTION_DEST_CONSTANTS_SCRATCH_HEAP;
						instruction.a_index = address;
					}
					else if("C" == src_tokens[0])
					{
						instruction.a_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
						instruction.a_index = address;
					}
					else if("V" == src_tokens[0])
					{
						instruction.a_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
						instruction.a_index = address;
					}
				}

				if(true == answer_requires_scratch_heap)
				{
					instruction.out_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
					scratch_heap[ordered_term_index].push_back(quaternion_t<double>());
					instruction.out_index = scratch_heap[ordered_term_index].size() - 1;

					ostringstream out;
					out << static_cast<long unsigned int>(scratch_heap[ordered_term_index].size() - 1);
					tokens[i-1] = "SH#";
					tokens[i-1] += out.str();
				}

				// setup b
				if("c" == lower_string(tokens[i+1]))
				{
					instruction.b_type = TOKENIZED_INSTRUCTION_DEST_C;
				}
				else if("z" == lower_string(tokens[i+1]))
				{
					instruction.b_type = TOKENIZED_INSTRUCTION_DEST_Z;
				}
				else
				{
					vector<string> src_tokens = STLStrTok("#", tokens[i+1]);

					istringstream in(src_tokens[1]);
					size_t address = 0;
					in >> address;

					if("SH" == src_tokens[0])
					{
						instruction.b_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
						instruction.b_index = address;
					}
					else if("CSH" == src_tokens[0])
					{
						instruction.b_type = TOKENIZED_INSTRUCTION_DEST_CONSTANTS_SCRATCH_HEAP;
						instruction.b_index = address;
					}
					else if("C" == src_tokens[0])
					{
						instruction.b_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
						instruction.b_index = address;
					}
					else if("V" == src_tokens[0])
					{
						instruction.b_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
						instruction.b_index = address;
					}
				}

				instructions[ordered_term_index].push_back(instruction);

				tokens.erase(tokens.begin() + i + 1);
				tokens.erase(tokens.begin() + i);

				i = 0;
			}
		}

		for(size_t i = 0; i < tokens.size(); i++)
		{
			if("+" == tokens[i] || "-" == tokens[i])
			{
				tokenized_instruction instruction;

				if("+" == tokens[i])
					instruction.f = &quaternion_math_base<double>::add;
				else
					instruction.f = &quaternion_math_base<double>::sub;

				bool answer_requires_scratch_heap = true;

				if("c" == lower_string(tokens[i-1]))
				{
					instruction.a_type = TOKENIZED_INSTRUCTION_DEST_C;
				}
				else if("z" == lower_string(tokens[i-1]))
				{
					instruction.a_type = TOKENIZED_INSTRUCTION_DEST_Z;
				}
				else
				{
					vector<string> src_tokens = STLStrTok("#", tokens[i-1]);

					istringstream in(src_tokens[1]);
					size_t address = 0;
					in >> address;

					if("SH" == src_tokens[0])
					{
						instruction.a_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
						instruction.a_index = address;
						instruction.out_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
						instruction.out_index = address;

						answer_requires_scratch_heap = false;
					}
					else if("CSH" == src_tokens[0])
					{
						instruction.a_type = TOKENIZED_INSTRUCTION_DEST_CONSTANTS_SCRATCH_HEAP;
						instruction.a_index = address;
					}
					else if("C" == src_tokens[0])
					{
						instruction.a_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
						instruction.a_index = address;
					}
					else if("V" == src_tokens[0])
					{
						instruction.a_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
						instruction.a_index = address;
					}
				}

				if(true == answer_requires_scratch_heap)
				{
					instruction.out_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
					scratch_heap[ordered_term_index].push_back(quaternion_t<double>());
					instruction.out_index = scratch_heap[ordered_term_index].size() - 1;

					ostringstream out;
					out << static_cast<long unsigned int>(scratch_heap[ordered_term_index].size() - 1);
					tokens[i-1] = "SH#";
					tokens[i-1] += out.str();
				}

				// setup b
				if("c" == lower_string(tokens[i+1]))
				{
					instruction.b_type = TOKENIZED_INSTRUCTION_DEST_C;
				}
				else if("z" == lower_string(tokens[i+1]))
				{
					instruction.b_type = TOKENIZED_INSTRUCTION_DEST_Z;
				}
				else
				{
					vector<string> src_tokens = STLStrTok("#", tokens[i+1]);

					istringstream in(src_tokens[1]);
					size_t address = 0;
					in >> address;

					if("SH" == src_tokens[0])
					{
						instruction.b_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
						instruction.b_index = address;
					}
					else if("CSH" == src_tokens[0])
					{
						instruction.b_type = TOKENIZED_INSTRUCTION_DEST_CONSTANTS_SCRATCH_HEAP;
						instruction.b_index = address;
					}
					else if("C" == src_tokens[0])
					{
						instruction.b_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
						instruction.b_index = address;
					}
					else if("V" == src_tokens[0])
					{
						instruction.b_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
						instruction.b_index = address;
					}
				}

				instructions[ordered_term_index].push_back(instruction);

				tokens.erase(tokens.begin() + i + 1);
				tokens.erase(tokens.begin() + i);

				i = 0;
			}
		}

		/*
		for(size_t i = 0; i < tokens.size(); i++)
		cout << tokens[i] << " ";

		cout << endl << endl;
		*/

		tokenized_instruction instruction;
		instruction.f = &quaternion_math_base<double>::copy;

		// setup a
		if("c" == lower_string(tokens[0]))
		{
			instruction.a_type = TOKENIZED_INSTRUCTION_DEST_C;
		}
		else if("z" == lower_string(tokens[0]))
		{
			instruction.a_type = TOKENIZED_INSTRUCTION_DEST_Z;
		}
		else
		{
			vector<string> src_tokens = STLStrTok("#", tokens[0]);

			istringstream in(src_tokens[1]);
			size_t address = 0;
			in >> address;

			if("SH" == src_tokens[0])
			{
				instruction.a_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
				instruction.a_index = address;
			}
			else if("CSH" == src_tokens[0])
			{
				instruction.a_type = TOKENIZED_INSTRUCTION_DEST_CONSTANTS_SCRATCH_HEAP;
				instruction.a_index = address;
			}
			else if("C" == src_tokens[0])
			{
				instruction.a_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
				instruction.a_index = address;
			}
			else if("V" == src_tokens[0])
			{
				instruction.a_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
				instruction.a_index = address;
			}
		}


		instruction.out_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
		instruction.out_index = ordered_term_index;

		instructions[ordered_term_index].push_back(instruction);
	}

	void SetupFinalZCopy(const string &src_destination)
	{
		string destination = lower_string(src_destination);

		if("z" == destination)
		{
			tokenized_instruction instruction;

			instruction.f = &quaternion_math_base<double>::copy;
			instruction.a_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
			instruction.a_index = answers.size() - 1;
			instruction.b_type = TOKENIZED_INSTRUCTION_DEST_NULL;
			instruction.out_type = TOKENIZED_INSTRUCTION_DEST_Z;

			instructions[answers.size() - 1].push_back(instruction);
			cout << "z push" << endl;
		}
		else
		{
			quaternion_t<double> copy_mask;

			for(size_t j = 2; j < destination.length(); j++)
			{
				if('x' == destination[j])
					copy_mask.x = 1.0;
				else if('y' == destination[j])
					copy_mask.y = 2.0;
				else if('z' == destination[j])
					copy_mask.z = 3.0;
				else
					copy_mask.w = 4.0;
			}

			scratch_heap[answers.size() - 1].push_back(copy_mask);

			tokenized_instruction instruction;

			instruction.f = &quaternion_math_base<double>::copy_masked;
			instruction.a_type = TOKENIZED_INSTRUCTION_DEST_ANSWER;
			instruction.a_index = answers.size() - 1;
			instruction.b_type = TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP;
			instruction.b_index = scratch_heap[answers.size() - 1].size() - 1;
			instruction.out_type = TOKENIZED_INSTRUCTION_DEST_Z;

			instructions[answers.size() - 1].push_back(instruction);
		}
	}

	bool is_valid_destination(const string &src_string);
	bool is_variable_swizzle(const string &src_string);
	bool is_constant_swizzle(const string &src_string);
	bool is_raw_swizzle(const string &src_string);
	bool is_function(const string &src_string);
	bool is_math_operator(const string &src_string);
	bool is_constant(const string &src_string);
	bool is_variable(const string &src_string);
	bool is_real_number(const string &src_string);
	bool is_constant_quaternion(const string &src_string);
	bool is_variable_quaternion(const string &src_string);
	bool CleanupEquationTokens(vector<string> &equation);
	void GetTerms(vector<string> src_equation, vector<term> &ordered_terms);
	bool compile_ordered_terms(const vector<term> &ordered_terms);

	bool assemble_compiled_instructions(void)
	{
		size_t instruction_count = 0;

		for(size_t i = 0; i < instructions.size(); i++)
			instruction_count += instructions[i].size();
		
		// lala debug
//		cout << instruction_count << endl;

		try
		{
			execution_stack = new assembled_instruction[instruction_count];
		}
		catch(...)
		{
			return false;
		}

		execution_stack_size = instruction_count;

		size_t execution_stack_index = 0;

		for(size_t i = 0; i < instructions.size(); i++)
		{
			for(size_t j = 0; j < instructions[i].size(); j++)
			{
				execution_stack[execution_stack_index].f = instructions[i][j].f;

				// do a
				switch(instructions[i][j].a_type)
				{
				case TOKENIZED_INSTRUCTION_DEST_ANSWER:
					{
						execution_stack[execution_stack_index].a = &answers[instructions[i][j].a_index];
						break;
					}
				case TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP:
					{
						execution_stack[execution_stack_index].a = &scratch_heap[i][instructions[i][j].a_index];
						break;
					}
				case TOKENIZED_INSTRUCTION_DEST_CONSTANTS_SCRATCH_HEAP:
					{
						execution_stack[execution_stack_index].a = &constant_scratch_heap[instructions[i][j].a_index];
						break;
					}
				case TOKENIZED_INSTRUCTION_DEST_Z:
					{
						execution_stack[execution_stack_index].a = &Z;
						break;
					}
				case TOKENIZED_INSTRUCTION_DEST_C:
					{
						execution_stack[execution_stack_index].a = &C;
						break;
					}
				default: // no nulls allowed in A
					{
						if(0 != execution_stack)
							delete [] execution_stack;

						return false;
					}
				}

				// do b
				switch(instructions[i][j].b_type)
				{
				case TOKENIZED_INSTRUCTION_DEST_ANSWER:
					{
						execution_stack[execution_stack_index].b = &answers[instructions[i][j].b_index];
						break;
					}
				case TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP:
					{
						execution_stack[execution_stack_index].b = &scratch_heap[i][instructions[i][j].b_index];
						break;
					}
				case TOKENIZED_INSTRUCTION_DEST_CONSTANTS_SCRATCH_HEAP:
					{
						execution_stack[execution_stack_index].b = &constant_scratch_heap[instructions[i][j].b_index];
						break;
					}
				case TOKENIZED_INSTRUCTION_DEST_Z:
					{
						execution_stack[execution_stack_index].b = &Z;
						break;
					}
				case TOKENIZED_INSTRUCTION_DEST_C:
					{
						execution_stack[execution_stack_index].b = &C;
						break;
					}
				case TOKENIZED_INSTRUCTION_DEST_NULL:
					{
						execution_stack[execution_stack_index].b = 0;
						break;
					}
				default: // invalid address
					{
						if(0 != execution_stack)
							delete [] execution_stack;

						return false;
					}
				}




				// do out
				switch(instructions[i][j].out_type)
				{
				case TOKENIZED_INSTRUCTION_DEST_ANSWER:
					{
						execution_stack[execution_stack_index].out = &answers[instructions[i][j].out_index];
						break;
					}
				case TOKENIZED_INSTRUCTION_DEST_TERM_SCRATCH_HEAP:
					{
						execution_stack[execution_stack_index].out = &scratch_heap[i][instructions[i][j].out_index];
						break;
					}
				case TOKENIZED_INSTRUCTION_DEST_CONSTANTS_SCRATCH_HEAP:
					{
						execution_stack[execution_stack_index].out = &constant_scratch_heap[instructions[i][j].out_index];
						break;
					}
				case TOKENIZED_INSTRUCTION_DEST_Z:
					{
						execution_stack[execution_stack_index].out = &Z;
						break;
					}
				case TOKENIZED_INSTRUCTION_DEST_C: // not allowed in OUT
					{
						if(0 != execution_stack)
							delete [] execution_stack;

						return false;
					}
				default: // no nulls allowed in OUT
					{
						if(0 != execution_stack)
							delete [] execution_stack;

						return false;
					}
				}

				execution_stack_index++;
			}
		}

		return true;
	}


	quaternion_t<double> Z, C;
	double a;

	quaternion_math_base<double> *q_math;
	string unique_formula_string;

	vector< quaternion_t<double> > answers;

	vector< quaternion_t<double> > constant_scratch_heap; // lala: use this for storing c and c swizzles used in variable expressions

	vector<	vector<tokenized_instruction> > instructions;
	vector< vector< quaternion_t<double> > > scratch_heap;

	assembled_instruction *execution_stack;
	size_t execution_stack_size;

	vector<function_mapping> function_map;
};

#endif