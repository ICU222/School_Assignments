#ifndef _Automata_h

#define _Automata_h


#include <iostream>
#include <string>
#include <algorithm>


using namespace std;


//============eNFA =====================
const int enfa_state = 11;	// �ٸ� FA �����, ���� const int�� ������ �����ؾ� �մϴ�.
const int enfa_of_input = 2;
const int enfa_trans_func = 13;
const int max_end_state = 10;

//=================DFA==================
int dfa_trans_func = 0;
int dfa_state = 0;
int dfa_end_state = 0;

//==================NFA====================
// eNFA, DFA�� ���� ���� �ٸ� NFA�Դϴ�. 3���� 6�������� �ִ� ���丶Ÿ�Դϴ�. 
const int nfa_state = 4;
const int nfa_input = 2;
const int nfa_trans_f = 8;
const int nfa_ed_state = 1;

// ================== queue ==================
int head = 0;
int tail = 0;


/*======== ���丶Ÿ�� ����ü ======     */
// eNFA, DFA, NFA ��� transition func�� �ٸ� ���� �����ϸ�
//  �⺻������ ���� ������ ���� ����ü�� �����Ͽ����ϴ�. 

typedef struct {
	string src_state;
	string input;
	string dst_state;

}trans_func;


typedef struct {
	string start_state;

	// end state�� 10����� ����
	string end_state[max_end_state];

	// input�� 2����� ���� ( �۽Ǹ� ���� )
	string input[2];

	string* state;

	trans_func* trans_f;


} Automata;


/* =========== ť �Լ� ==================*/
void push(string s[], string input_str) {
	s[tail++] = input_str;
}

string pop(string* s) {
	return s[head++];
}


//==================== �Լ� ==========================

/* ť�� �ش� ��Ʈ���� �����ϴ��� Ȯ�� */
bool not_in_queue(string q[], string s) {
	// tail�� �����Ƿ� ���� ���ڸ� ���� �ȾƵ� �ȴ�. 
	bool result = true;

	//tail�� ť�� ������ ��Ʈ���� ����Ű�� �����Ƿ�
	for (int x = 0; x <= tail; x++)
		if (q[x] == s)
			result = false;

	return result;

}


/*  �־��� state(ex : ACFG)�� end state�� �ϳ��� �����ϴ��� Ȯ���ϴ� �Լ� */
bool is_contain_end_state(string str, string end[], int end_len) {

	for (unsigned int t = 0; t < str.length(); t++) {
		for (int y = 0; y < end_len; y++) {

			// �� end state�� �����ϸ� ������ ��ġ(index)�� ��ȯ�ϹǷ� �� �ȿ� ������ true
			// �̻��ϰ� �� ��Ʈ���� find�� ������ 0�� ���ͼ� �̸� ���̶�� �ν��ؼ� �߰� ���� end[y].length() != 0 ����
			if (str.find(end[y]) >= 0 && str.find(end[y]) < str.length() && end[y].length() != 0)
				return true;
		}
	}


	return false;
}

/* �־��� state�� closure�� ���Ͽ� result�� ���� */
void Closure(Automata a, string src, string* result) {


	int not_in_result = 1;

	// ó�� state �� ���� �ϳ��϶�
	// �� �̰� ��� ȣ��Ǵϱ� ABCD => A => TF(A,@) = C�϶�, result�� A�� �ִ� ����
	// �׷��� ������ result�� ����� �ְ� ���� ���̴�. 
	if (src.length() == 1) {

		for (unsigned int y = 0; y < (*result).length(); y++) {
			if ((*result).substr(y, 1) == src)
				not_in_result = 0;
		}
		if (not_in_result) {

			(*result).append(src);
		}
	}

	// input state �̸��� abvdf�̷��� �� ��
	else {
		for (unsigned int e = 0; e < src.length(); e++) {
			// �ϴ� �� �־� �װ� closure�ϱ�
			(*result).append(src.substr(e, 1));
		}

	}

	// ���� ���� state���� @�� �� �� �� �ִ� state�� �����Ѵٸ�
	for (unsigned int f = 0; f < src.length(); f++) {
		for (unsigned int r = 0; r < enfa_trans_func; r++) {
			if (a.trans_f[r].src_state == src.substr(f, 1) && a.trans_f[r].input == "@") {
				// �� ���¿� ���� closure ����
				Closure(a, a.trans_f[r].dst_state, result);
			}
		}
	}

	sort((*result).begin(), (*result).end());

}

/* src_state,input ------use transition_function --------->> find dst_state  */
string transition_output(Automata a, string src_state, string input, int num_tf) {

	string res;
	int t = 0;
	for (unsigned int f = 0; f < src_state.length(); f++) {
		for (int r = 0; r < num_tf; r++) { 
			// �� �־��� string�� ����(����) �� input�� ���� transition�� �����Ѵٸ�
			if (a.trans_f[r].src_state == src_state.substr(f, 1) && a.trans_f[r].input == input)
				// �� output state�� ����
				res.append(a.trans_f[r].dst_state);
		}
	}
	// ����� output state���� ��ȯ
	return res;

}

/* eNFA ---> DFA */
Automata Subset_Construction(Automata a) {

	Automata result_dfa;
	result_dfa.input[0] = a.input[0];
	result_dfa.input[1] = a.input[1];

	// alloc�� ��ģ���̴� �׳� ����� �Űܶ�
	trans_func res_trans_func[64];

	// ��� ����� ��Ʈ��
	string result_state0;
	string result_state1;
	string poped_str;

	string queue[1 << enfa_state]; // ���� ���°� 6����� �ִ� 2^6 = 64���̹Ƿ�


	// eNFA -> DFA 1�ܰ� : ����state�� closure�� ���ۻ����̴�.  
	// ���� ���¸� result_state0�� ����
	Closure(a, a.start_state, &result_state0);
	push(queue, result_state0);

	result_dfa.start_state = result_state0;

	// eNFA -> DFA 2�ܰ� : ���� �������� subset construction ����
	while (head != tail) {

		result_state0.clear();
		result_state1.clear();

		// �̰��� ������ 
		poped_str = pop(queue);

		// input�� 2���� ���������Ƿ� 
		Closure(a, transition_output(a, poped_str, a.input[0],enfa_trans_func), &result_state0);
		Closure(a, transition_output(a, poped_str, a.input[1], enfa_trans_func), &result_state1);


		// Clousure1 (input is 0 ) >> �� ť�� ���� ���ο� �����̸�, ���̰� 1 �̻��� ��Ʈ�� ��ȯ��
		if (result_state0.length() != 0) {

			res_trans_func[dfa_trans_func].src_state = poped_str;
			res_trans_func[dfa_trans_func].input = a.input[0];
			res_trans_func[dfa_trans_func++].dst_state = result_state0;

			// �� ��� ��Ʈ���� end state�� �����ϸ�
			if (is_contain_end_state(result_state0, a.end_state, sizeof(a.end_state) / sizeof(string)))
				// DFA�� end state�� ����
				result_dfa.end_state[dfa_end_state++] = result_state0;

			// ť�� ���� state��� ť�� ����
			if (not_in_queue(queue, result_state0)) {
				push(queue, result_state0);
			}
		}


		// Clousure2 (input is 1 )   >> �� ť�� ���� ���ο� �����̸�, ���̰� 1 �̻��� ��Ʈ�� ��ȯ��
		if (result_state1.length() != 0) {

			res_trans_func[dfa_trans_func].src_state = poped_str;
			res_trans_func[dfa_trans_func].input = a.input[1];
			res_trans_func[dfa_trans_func++].dst_state = result_state1;

			// �� ��� ��Ʈ���� end state�� �����ϸ�
			if (is_contain_end_state(result_state1, a.end_state, sizeof(a.end_state) / sizeof(string)))
				// DFA�� end state�� ����
				result_dfa.end_state[dfa_end_state++] = result_state1;


			if (not_in_queue(queue, result_state1)) {
				push(queue, result_state1);
			}
		}
	}

	/*Queue�� DFA�� ���µ��� ������ ���̹Ƿ� result_dfa�� �����Ѵ�.*/
	result_dfa.state = new string[tail];
	for (int r = 0; r < tail; r++) {
		result_dfa.state[r] = queue[r];
		dfa_state++;
	}

	/* res_tran_func�� DFA�� transition function�� ������ ���̹Ƿ� result_dfa�� �����Ѵ�.*/
	result_dfa.trans_f = new trans_func[dfa_trans_func];
	for (int r = 0; r < dfa_trans_func; r++) {
		result_dfa.trans_f[r].src_state = res_trans_func[r].src_state;
		result_dfa.trans_f[r].input = res_trans_func[r].input;
		result_dfa.trans_f[r].dst_state = res_trans_func[r].dst_state;
	}

	return result_dfa;
}

/* print NFA with ( state, input, transition_function, start_state, end_state ) */
void print_eNFA(Automata a) {

	cout << "\nstate :	";
	for (int i = 0; i < enfa_state ; i++)
		cout << a.state[i] << " ";

	cout << "\ninput :		" << a.input[0] << ", " << a.input[1] << endl;

	cout << "start state :	" << a.start_state << endl;

	cout << "end state :	";
	for (int t = 0; t <  max_end_state; t++)
		cout << a.end_state[t] << " ";

	cout << "\ntransition function : \n";
	for (int i = 0; i < enfa_trans_func ; i++)
		cout << "	delta (" << a.trans_f[i].src_state << "," << a.trans_f[i].input << ") = " << a.trans_f[i].dst_state << endl;



}

/* print DFA with ( state, input, transition_function, start_state, end_state ) */
void print_DFA(Automata a) {

	cout << "\nstate :	";
	for (int i = 0; i < dfa_state; i++)
		cout << a.state[i] << " ";

	cout << "\ninput :		" << a.input[0] << ", " << a.input[1] << endl;

	cout << "start state :	" << a.start_state << endl;

	cout << "end state :	";
	for (int t = 0; t < sizeof(a.end_state) / sizeof(string); t++)
		cout << a.end_state[t] << " ";

	cout << "\ntransition function : \n";
	for (int i = 0; i < dfa_trans_func; i++)
		cout << "	delta (" << a.trans_f[i].src_state << "," << a.trans_f[i].input << ") = " << a.trans_f[i].dst_state << endl;


}


/* state�� �� �ϳ��� �����϶�  output�� ã�� �Լ�  */
string find_output(Automata a, string src, string input) {

	string dst;

	for (int r = 0; r < enfa_trans_func; r++) 
		if (a.trans_f[r].src_state == src && a.trans_f[r].input == input)
			dst = a.trans_f[r].dst_state;

	return dst;
}

/* for DFA string */
void is_str_accepted(Automata res, string input_str) {

	string state;

	state = find_output(res, res.start_state, input_str.substr(0, 1));
	
	if (state.length() == 0) {
		cout << "string is rejected....\n";
		return;
	}

	else {
		for (unsigned int t = 1; t < input_str.length() ; t++) {
			state = find_output(res, state, input_str.substr(t, 1));
			//cout << " qt : " << state << endl;
			if (state.length() == 0) {
				cout << "string is rejected....\n";
				return;
			}
		}
	}

	for (int t = 0; t < sizeof(res.end_state) / sizeof(string); t++) {
		if (state.compare(res.end_state[t]) == 0) {
			cout << "string is Accepted !!! \n ";
			return;
		}
	}

	cout << "string is rejected....\n";

}


/* for NFA string */
void check_str_in_nfa(Automata res, string input_str, string start_state) {

	string state;
	
	state = transition_output(res, start_state, input_str.substr(0, 1), nfa_trans_f);


	for (unsigned int r = 1; r < input_str.length(); r++) {
		if (state.length() == 0) {
			cout << "string is rejected....\n";
			return;
		}

		else 
			state = transition_output(res, state, input_str.substr(r, 1), nfa_trans_f);
	

		for (int t = 0; t < sizeof(res.end_state) / sizeof(string); t++) {
			if (state.find(res.end_state[t]) >= 0 && state.find(res.end_state[t]) < state.length() && res.end_state[t].length() != 0) {
				cout << "string is Accepted !!! \n ";
				return;
			}
		}

	}
	cout << "string is rejected....\n";
}

#endif