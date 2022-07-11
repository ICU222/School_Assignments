
#include <iostream>
#include <string>
using namespace std;

int get_Expr_idx(string); // �Ŀ� . ������ -1 ����
int get_endExpr_idx(string,int); // 
int get_Expr_len(int,int); //
int get_cutExpr_idx(string, int, int);
void SKI_translater(string&);
void translate_S(string&, int, int);
void translate_K(string&, int);
void translate_I(string&, int);
int Q_delete_paren(string);
void beta_conversion(string&);


int main() {

	string lambda_expr;
	
	cout << " ====== Lamba expression to SKI expression ======\n\n";
	cout << " ���� �� : /f./x.f(fx) \n";
	cout << " ǥ�� �� : Lf.Lx.f(fx) \n";
	cout << "\n ���� �� : (/m n f x -> m f (n f x)) (/f x -> f x) (/f x -> f x) \n";
	cout << " ǥ�� �� : (Lm.Ln.Lf.Lx.mf(nfx))(Lf.Lx.fx)(Lf.Lx.fx)\n";

	cout << "\n\n ���� ���� �Է����ּ��� : ";
	cin >> lambda_expr;

	///*
	if(lambda_expr[0] == '(')
		beta_conversion(lambda_expr);
	
	SKI_translater(lambda_expr);

	cout << "\n\n RESULT : \n\n";
	cout << lambda_expr << endl;
	//*/
	system("pause");
	return 0;
}

//���� : L, �� ���� �������� ������ ���Ƿ�, ���� ������ L�� ã�� �� ��ġ���� +1�� �ϸ� 
// ���� ������ ������ ��ġ�� ã�� �� �ִ�. 
int find_lesftmost_lambda_var(string s) {

	for (unsigned int l_index = 0;  l_index < s.length(); l_index++)
		if (s[l_index] == 'L')
			return  l_index + 1;

	return -1;
}

// ���� �Ŀ��� '.' ������ �� ��  ������ ��Ÿ���� ��� ��ġ�� ��Ʈ���� ����
// ex : Lx.yyy(x)yxx >> L�� �ε����� 0�̶�� �Ҷ�, 7,10,11 �� ��Ʈ���� ����
void find_var_inExpr(string s, int var_idx, int dotpos, int endpos , int* var_index, int* num) {

	int k = 0;

	for (int t = dotpos + 1; t <= endpos; t++) {
		// Lx.Ly.yyx ���� L'x' == yy'x'�϶�
		if (s[t] ==s[var_idx]) {
			var_index[k++] = t;
		}
	}
	*num = k;
}

// ��Ÿ ������ ������ ���� ���� '.' ��ġ�� ã�´�.
int beta_find_dot_idx(string s, int endpos) {

	for (int t = endpos; t >= 0; t--) {
		if (s[t] == '.')
			return t;
	}

	return -1;
}

// ��Ÿ ������ ������ ���� ���� �� ��ġ�� ã�´�. 
// (Lx.fx)(Ld.d) => 6 �� ')'�� �ε��� ����
int beta_find_end_idx(string s) {

	int pair_paren = 0;

	for (unsigned int y = 0; y < s.length(); y++) {
		if (s[y] == '(') 
			pair_paren++;
		
		if (s[y] == ')') {
			pair_paren--;
			if (pair_paren == 0)
				return y;
		}
	}
	return -1;
}

// beta conversion  �� ����Ǵ� ��Ÿ ���� ���� ���� index�� ���Ѵ�. ')'�� index return 
int beta_find_beta_endpos(string s, int endpos) {
	
	// (Lx.f)(Lx.x) >> endpos: 4 (f�� ��ġ) + 2 = beta�� 'L'�� ��ġ
	int pair_paren = 0;

	for (unsigned int t = endpos + 1; t < s.length(); t++) {
		if (s[t] == '(')
			pair_paren++;

		if (s[t] == ')') {
			pair_paren--;
			if (pair_paren == 0) {
				return t;
			}
		}

	}

	// ���� �� beta���� ���� �� ���� ���� �� �� index return 
	return s.length() - 1;
}



void beta_conversion(string& s) {
	/*
	1. ���� ������ ã�´�. '  var_idx
	2. ���� ���� ���� ã�´�.  endpos
	3. ���� ���� '.'�� ã�´�. dotpos
	4. ���� ���� ���� ��ġ�� ã�´�.  find_var_inEXPR
	5. ���� ����
	6. beta conversion ���� ������ ()�� �����ش�. 
	*/
	int var_idx = find_lesftmost_lambda_var(s);
	int endpos = beta_find_end_idx(s);
	int dotpos = beta_find_dot_idx(s, endpos);
	int endbeta_pos = beta_find_beta_endpos(s, endpos);
	int num_of_var = 0;

	string beta;
	int var_loc[10];

	// beta conversion ����
	// �� �̻� beta conversion �� ���� ������ ����
	while (endpos != s.length()-1) {

		// ���� ���� beta conversion�� ������ �������� ��ġ�� ã�´�. 
		find_var_inExpr(s, var_idx, dotpos, endpos, var_loc, &num_of_var);
	


		// beta ���� �ڸ���. (��Ÿ ��) ��ü ����
		// ex) (Lm.Ln.Lf.Lx.mf(nfx))(Lf.Lx.fx)(Lf.Lx.fx) >> (Lf.Lx.fx) ����
		beta = s.substr(endpos + 1, endbeta_pos - endpos );
		


		// ��ü �Ŀ��� ��Ÿ ���� ����
		//ex) (Lm.Ln.Lf.Lx.mf(nfx))(Lf.Lx.fx)(Lf.Lx.fx) >> (Lm.Ln.Lf.Lx.mf(nfx))(Lf.Lx.fx)
		s.erase(endpos + 1, endbeta_pos - endpos );




		// beta conversion�� ����Ǵ� ���� ���� ������ ��ġ�� beta ����
		// ex) (Lm.Ln.Lf.Lx.mf(nfx))(Lf.Lx.fx) >> (Lm.Ln.Lf.Lx.(Lf.Lx.fx)f(nfx))(Lf.Lx.fx)
		for (int r = 0; r < num_of_var; r++) {
			s.erase(var_loc[r], 1);
			s.insert(var_loc[r], beta);
			
		}

		// ���� ���� ��ü �Ŀ��� ����
		//(Lm.Ln.Lf.Lx.(Lf.Lx.fx)f(nfx))(Lf.Lx.fx) >> (Ln.Lf.Lx.(Lf.Lx.fx)f(nfx))(Lf.Lx.fx)
		s.erase(1, 3);

		var_idx = find_lesftmost_lambda_var(s);
		endpos = beta_find_end_idx(s);
		dotpos = beta_find_dot_idx(s, endpos);
		endbeta_pos = beta_find_beta_endpos(s, endpos);
		num_of_var = 0;

	}
	

	// ���� �� �� ()�� �����ؾ� �ȴ�. 
	// ex) (Lf.Lx.(Lf.Lx.fx)f((Lf.Lx.fx)fx)) >> Lf.Lx.(Lf.Lx.fx)f((Lf.Lx.fx)fx) 
	s.erase(s.length() - 2, 1);
	s.erase(0, 1);

}




// ������-> �������� ó�� ��Ÿ���� ���� ���� '.' ��ġ ��ȯ
int get_Expr_idx(string s) {

	// ���� �����ʿ������� '.'�� ��ġ�� ã�´�. ( ���� ���� �����ؾߵǴ� ���� ã�� ���� )
	for (int idx = s.length() - 1; idx >= 0 ; idx--) 
		if (s[idx] == '.')
			return idx;

	// ���� .�� ���ٸ� ( ��� ���� SKI�� ����Ǿ��ų� .�� ���� ���� ���(����) )
	return -1;

}

// ���� ���� �����ϴ� ���� ���� �� index�� ��ȯ
int get_endExpr_idx(string s, int dotpos){

	//int end_idx = 0;
	unsigned int t = dotpos;
	int paren = 0;

	while(t < s.length()){

		if (s[t] == '(') {
			paren += 1;

		}

		if (s[t] == ')') {
			paren -= 1;
			if (paren == -1)
				return t-1;
		}
		
		t++;
	}

	return  t-1;
}

// '.' ������ ���� ���� ���� ��ȯ
int get_Expr_len(int start, int end){

	return (end - start);
}

// '.'������ ���� ������ �ɰ��µ�, �ɰ� �͵� �� �� ���� ������ idx ��ȯ 
/*  ����
 Lx.(aa)(bb) , '.' = 2  ---> return 7
 Lx.aaaaaaaa , '.' = 2  ---> return 10
 Lx.(aaa(a)a(a))aa, '.' = 2  ---> return 15
 Lx.aaa(a)a(a)aa, '.' = 2  ---> return 13
 Lx.aaaaa(bb) , '.' = 2  ---> return 8
 Lx.(aaaaads)as(dsa(bb)),   '.' = 2  --->  14 ��ȯ 
 Lx.(aaaaads)as(d(s)aa(bb)), '.' = 2  --->   14 ��ȯ  
*/
int get_cutExpr_idx(string s, int dotpos, int endpos) {

	int pair_paren = 0;

	// �� �ڰ� )�� �����ٸ� : �� )�� ���̴� (�� ��ġ�� ã�´�. ���߿� ������ ()�� �����ϴ� ���
	// ex: Lx.dsadasd(fdfdfd(dsfsdfds(fdsfsd)sdfsdf)dsfdsfsd) >> Lx.dsadasd <-�� ���� �ڸ� ��ȯ
	if (s[endpos] == ')'){

		pair_paren++;

		for (int i = endpos - 1; i >= dotpos; i--) {
			if (s[i] == '(') {
				pair_paren--;
				
				if(pair_paren ==0)
					// �� ��� () ���� ��������� >> �̶� '('�� �߸��� ���� ���� �ٱ��� ��ȣ��. 
					//  == ó�� if���� ')'�� �����Ǵ� '(' 
					return i;
			}
			if(s[i] == ')')
				pair_paren++;
		}
	}

	// �� �ڰ� )�� ������ �ʴ´�. >> )�� ���ö����� �ݺ�
	//ex: Lx.dsadas(fdsfsdf)fdsfsd >>   dsadas(fdsfsdf)   fdsfsd    >> ���� �ڸ� index ��ȯ
	else {
		for (int i = endpos - 1; i >= dotpos; i--) {
			if (s[i] == ')') {
				return i+1;
			}
		}
		return endpos;
	}
	return - 1;
}


int Q_delete_paren( string s) {
	
	int do_delete = 1;
	int rparen = 0;
	int check_error = 0;

	if (s[0] == '(' && s[s.length() - 1] == ')') {
		
		for (unsigned i = 1; i < s.length() - 1; i++) {
			if (s[i] == ')')
				check_error++;

			if (s[i] == '(')
				check_error--;
		}

		// �� �� ��,���� ()�� �����ص� ()�� ������ ��ġ�� �� 
		//  (Kf)(S(Kf)I) �� ���� �����ص� Kf)(S(Kf)I �� �����δ� ������. �̰� ��ƾߵ�
		if (check_error == 0) {
			
			for (unsigned i = 1; i < s.length() - 1; i++) {
				if (s[i] == ')') {
					do_delete--;
					if (do_delete == 0) {
						
						break;
					}
				}
				if(s[i] == '(')
					do_delete++;
			}
		}

		// �� ��,���� ()�� �����ϸ� ()�� ������ ��ġ���� ���� ��
		// (Kf)aaa >> Kf)aaa (ERROR)
		else {
			
			do_delete = 0;
		}
	}

	else {
		
		do_delete = 0;
	}

	
	return do_delete;

}


void SKI_translater(string& expr) {

	int dot_pos = 0;
	int end_expr_pos = 0;
	int expr_len = 0;
	int cut_pos = 0;

	// �� input���� ���� ��Ʈ���� '.'�� ������ �� ���� �ݺ�
	// get_Expr_idx�� ��Ʈ���� '.'�� ������ -1�� ��ȯ�ϹǷ�
	while (get_Expr_idx(expr) != -1) {
		dot_pos = get_Expr_idx(expr);
		end_expr_pos = get_endExpr_idx(expr, dot_pos);

		// �� ���� ���� ���̰� 1 �϶� >> K or I combinator
		if (get_Expr_len(dot_pos, end_expr_pos) == 1) {
			// Lx.x �� ���
			if (expr[dot_pos - 1] == expr[end_expr_pos])
				translate_I(expr, dot_pos);
			// Lx.y �� ���
			else
				translate_K(expr, dot_pos);
		}

		// �� ���� ���� ���̰� 2 �̻��϶� >> S combinator
		else
			translate_S(expr, dot_pos, end_expr_pos);


	}


}


// �� ���� �� >> S(���� ��)(���� ��)
// ...Lx.statment.. >> ...S(Lx.state a)(Lx.state b)....
void translate_S(string& s, int dotpos, int endpos) {

	int do_delete_paren = 0; // �߸� ���� ��ȣ�� �����ؾ��ϴ��� �Ǵ��ϴ� ����
	// ex. Lx.(fx)(fx) >> S(Lx.fx)(Lx.fx) �� �ٲ�� ���� (fx) -> fx �� ��ȣ�� �������ߵ�.

	int cut_pos = get_cutExpr_idx(s, dotpos, endpos); // ���� �ڸ��� ��ġ�� ����

	string lambda_variable = s.substr(dotpos - 1, 1); // ���� ���� (L������ ���� ���ĺ�) �� ã�Ƽ� ����

	string rest_statement = s.substr(endpos + 1, s.length() - 1);
	// S transform�� �������� �ʴ� ���� �ĵ��� ����

	string div_statement1 = "L";
	string div_statement2 = "L";
	string tmp_expr;


	tmp_expr = s.substr(dotpos + 1, (cut_pos - dotpos -1));
	
	
	do_delete_paren = Q_delete_paren(tmp_expr);

	// �߸� �Ŀ��� ()�� �����ؾ� �Ǵ��� �Ǵ�
	// ���� ()�� ���� ���ϸ� Lx.(fx)(fx) >> S(Lx.(fx))(Lx.(fx)) �̷��� ()�� ���ʿ��ϰ� ���� ���̰� ���꿡�� ����
	if (do_delete_paren) {
		tmp_expr = tmp_expr.substr(1, tmp_expr.length() - 2);
	}

	// S(�� ���� ����.)(Lx.xxxx)
	// div1 =  ( + L + x + . + <expr> )
	div_statement1 = "(" + div_statement1 + lambda_variable + "." + tmp_expr + ")";
	

	tmp_expr.clear();
	
	
	// ���� �����ϰ� �ݺ�  S(Lx.xxxx)(�� ���� ����� ����)
	tmp_expr = s.substr(cut_pos, (endpos- cut_pos + 1));
	

	do_delete_paren = Q_delete_paren(tmp_expr);

	if (do_delete_paren) {
		tmp_expr = tmp_expr.substr(1, tmp_expr.length() - 2);
	}

	// S(Lx.xxxx)(�� ���� ����.)
	// div2 =  ( + L + x + . + <expr> )
	div_statement2 = "(" + div_statement2 + lambda_variable + "." + tmp_expr + ")";
	

	// 3. s���� ������ ���� ���� ����
	s.erase(dotpos - 2, s.length());

	// ���� : �߸� ��ü ��(s) + ���� ��1 + ���� ��2 + lump
	// ex ) ......  +  S  + (Lx.xxx) + (Lx.yyy) + .....
	s = s + "S" + div_statement1 + div_statement2 + rest_statement;

	
}


// ...Lx.y... >> ...Ky...
void translate_K(string& s, int dotpos ){

	string lump;
	string ch = s.substr(dotpos + 1, 1);
	// ���� ���� ���ڸ� ����
	// ex) Ly.f >> f ����
	


	// Lx.x������ ... ���� ����
	lump = s.substr(dotpos + 2, s.length() - 1);

	// s���� ...Lx.x...�� Lx.x...�� ���� ����
	s.erase(dotpos - 2, s.length());

	// K�� �߰� �̰� ���� �ٲ�
	s = s + "K" + ch + lump;

}


// .....Lx.x.... >> .....I... 
void translate_I(string& s, int dotpos) {
	string lump;

	// Lx.x������ ... ���� ����
	lump = s.substr(dotpos + 2, s.length() - 1);

	// s���� ...Lx.x...�� Lx.x...�� ���� ����
	s.erase(dotpos - 2, s.length());

	s = s + "I" + lump;

	// (I) --> I ���� I�� s[dotpos-2] �� ��ġ
	if (s[dotpos - 3] == '(' && s[dotpos - 1] == ')')
		s = s.substr(0, (dotpos - 3)) + s.substr(dotpos - 2, 1) + s.substr(dotpos, (s.length() - dotpos));

}
