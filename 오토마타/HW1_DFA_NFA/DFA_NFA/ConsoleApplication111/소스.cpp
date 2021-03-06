#include "Automata.h"


void main() {

	Automata res;
	Automata eNFA;
	Automata NFA;

	string input_str;

	// 다른 오토마타를 실험해 보려면 여기 정보를 변경해야 됩니다. =-==============
	eNFA.start_state = "A";
	eNFA.end_state[0] = "K";
	eNFA.input[0] = "a";
	eNFA.input[1] = "b";

	string state_name[enfa_state] = { "A","B","C","D","E","F","G","H","I","J","K"};
	trans_func tf[enfa_trans_func] = {
		{ "A","@","H" }, // src_state , input , dst_state
		{ "A","@","B" },
		{ "B","@","C" },
		{ "B","@","E" },
		{ "C","a","D" },
		{ "D","@","G" },
		{ "E","b","F" },
		{ "F","@","G" },
		{ "G","@","H" },
		{ "G","@","B" },
		{ "H","a","I" },
		{ "I","b","J" },
		{ "J","b","K" }
	};
	eNFA.state = state_name;
	eNFA.trans_f = tf;

	//================================================

	NFA.start_state = "0";
	NFA.end_state[0] = "3";
	NFA.input[0] = "a";
	NFA.input[1] = "b";

	string state_name_nfa[nfa_state] = { "0","1","2","3"};
	trans_func tf_nfa[nfa_trans_f] = {
		{ "0","a","0" }, // src_state , input , dst_state
		{ "0","b","0" },
		{ "0","a","1" },
		{ "0","b","2" },
		{ "1","a","3" },
		{ "2","b","3" },
		{ "3","a","3" },
		{ "3","b","3" },
	};
	NFA.state = state_name_nfa;
	NFA.trans_f = tf_nfa;




	//===========================================================================
	

	

	cout << " epsilon NFA is : \n";
	print_eNFA(eNFA);

	res = Subset_Construction(eNFA);
	cout << "\n\n ====================== result =====================\n\n";
	cout << " DFA is :\n";
	print_DFA(res);


	
	cout << "\n\n===================================================\n\n";
	cout << "(for dfa ) 스트링을 입력해주세요 ( input : a or b   ex : abaabbb  ) :";
	cin >> input_str;

	is_str_accepted(res, input_str);


	input_str.clear();
	cout << "\n\n===================================================\n\n";
	cout << "(for nfa ) 스트링을 입력해주세요 ( input : a or b   ex : abaabbb  ) :";
	cin >> input_str;




	check_str_in_nfa(NFA, input_str, NFA.start_state);

	input_str.clear();
	

	delete[] res.state;
	delete[] res.trans_f;

}