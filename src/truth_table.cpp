/*******************
** 2012-02-25
** 离散数学真值表
*/
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <cctype>
#include <cmath>
using namespace std;

/*
** isoperator
** 判断是否是合法的操作符
*/
int isoperator(char c)
{
    switch (c)
    {
    case '!':
    case '&':
    case '|':
    case '>':
    case '#':
        return 1;
    }
    return 0;
}

/*
** 判断是否是双目操作符
*/
int isbinaryoperator(char c)
{
    return c != '!' && isoperator(c);
}

/*
** 判断是否是不合法字符
*/
int isinvalidchar(char c)
{
    if (c == '(' || c == ')')
        return 0;
    else if (isupper(c) || isoperator(c))
        return 0;
    return 1;
}

/*
** 判断是否是大写字母
*/
int isupperalpha(char c)
{
    return isupper(c);
}

/*
** priority
** 输出联结词的优先级
*/
int priority(char opr)
{
    switch (opr)
    {
    case '!':
        return 5;
    case '&':
        return 4;
    case '|':
        return 3;
    case '>':
        return 2;
    case '#':
        return 1;
    default:
        return 0;
    }
}

/*
** 判断表达式是否合法
*/
int is_valid_expression(const string& exp)
{
    //判断表达式是否为空
    if (exp.empty())
    {
        cout << "你什么也没输入！" << endl;
        return 0;
    }
    //判断表达式中是否有不合法的字符
    string::const_iterator pos;
    pos = find_if(exp.begin(), exp.end(), isinvalidchar);
    if (pos != exp.end())
    {
        cout << "无效的字符: '" << *pos << '\'' << endl;
        return 0;
    }
    //开头的字符要求
    if (!isupper(*(exp.begin())) && *(exp.begin()) != '(' && *(exp.begin()) != '!')
    {
        cout << "开头字符错误!" << endl;
        return 0;
    }
    //结尾的字符要求
    if (!isupper(*(exp.rbegin())) && *(exp.rbegin()) != ')')
    {
        cout << "结尾字符错误!" << endl;
        return 0;
    }
    //表达式中变元的个数至少为1
    if (0 == count_if(exp.begin(), exp.end(), isupperalpha))
    {
        cout << "没有变元!" << endl;
        return 0;
    }
    //左右括号数要相等
    if (count(exp.begin(), exp.end(), '(') != count(exp.begin(), exp.end(), ')'))
    {
        cout << "括号数不等!" << endl;
        return 0;
    }
    //每一对匹配的括号，右括号都在左括号后
    vector<int> bracketpos[2];
    for (pos = exp.begin(); pos != exp.end(); ++pos)
    {
        if (*pos == '(')
            bracketpos[0].push_back(distance(exp.begin(), pos));
        else if (*pos == ')')
            bracketpos[1].push_back(distance(exp.begin(), pos));
    }
    vector<int>::size_type v;
    for (v = 0; v != bracketpos[0].size(); ++v)
    {
        if (bracketpos[0][v] > bracketpos[1][v])
        {
            cout << "括号位置不匹配!" << endl;
            return 0;
        }
    }
    //每个字符左右应满足的条件
    string::size_type i;
    for (i = 0; i != exp.size() - 1; ++i)
    {
        if (isupper(exp[i]))
        {
            if (exp[i + 1] != ')' && !isbinaryoperator(exp[i + 1]))
            {
                cout << '\'' << exp[i] << "'右边有误!错误:" << exp[i] << exp[i + 1] << endl;
                return 0;
            }
        }
        else if (exp[i] == '(')
        {
            if (exp[i + 1] == ')' || isbinaryoperator(exp[i + 1]))
            {
                cout << '\'' << exp[i] << "'右边有误!错误:" << exp[i] << exp[i + 1] << endl;
                return 0;
            }
        }
        else if (exp[i] == ')')
        {
            if (exp[i + 1] != ')' && !isbinaryoperator(exp[i + 1]))
            {
                cout << '\'' << exp[i] << "'右边有误!错误:" << exp[i] << exp[i + 1] << endl;
                return 0;
            }
        }
        else if (exp[i] == '!')
        {
            if (exp[i + 1] == ')' || isbinaryoperator(exp[i + 1]))
            {
                cout << '\'' << exp[i] << "'右边有误!错误:" << exp[i] << exp[i + 1] << endl;
                return 0;
            }
        }
        else
        {
            if (exp[i + 1] == ')' || isbinaryoperator(exp[i + 1]))
            {
                cout << '\'' << exp[i] << "'右边有误!错误:" << exp[i] << exp[i + 1] << endl;
                return 0;
            }
        }
    }
    return 1;
}

/*
** 获取表达式中的变元信息
*/
set<char> get_expinfo(const string& exp)
{
    set<char> exp_elem;

    for (string::size_type i = 0; i != exp.size(); ++i)
    {
        if (isupper(exp[i]))
        {
            exp_elem.insert(exp[i]);
        }
    }
    return exp_elem;
}

/*
** 中缀转后缀
** 算法
** 1)检查输入的下一元素。
** 2)假如是个操作数，输出。
** 3)假如是个开括号，将其压栈。
** 4)假如是个运算符，则
**	  i) 假如栈为空，将此运算符压栈。
** 	 ii) 假如栈顶是开括号，将此运算符压栈。
** 	iii) 假如此运算符比栈顶运算符优先级高，将此运算符压入栈中。
** 	 iv) 否则栈顶运算符出栈并输出，重复步骤4。
** 5)假如是个闭括号，栈中运算符逐个出栈并输出，直到遇到开括号。开括号出栈并丢弃。
** 6)假如输入还未完毕，跳转到步骤1。
** 7)假如输入完毕，栈中剩余的所有操作符出栈并输出它们。
*/
string infix_to_suffix(const string& exp)
{
    char top;
    stack<char> s;
    string suffix;

    for (string::size_type i = 0; i != exp.size(); ++i)
    {
        if (isupper(exp[i]))
            suffix += exp[i];
        else if (exp[i] == '(')
            s.push(exp[i]);
        else if (isoperator(exp[i]))
        {
            while (1)
            {
                if (s.empty() || s.top() == '(')
                {
                    s.push(exp[i]);
                    break;
                }
                else
                {
                    top = s.top();
                    if (priority(exp[i]) > priority(top) || (exp[i] == '!' && top == '!'))
                    {
                        s.push(exp[i]);
                        break;
                    }
                    else
                    {
                        suffix += s.top();
                        s.pop();
                    }
                }
            }
        }
        else if (exp[i] == ')')
        {
            while (!s.empty() && (top = s.top()) != '(')
            {
                suffix += top;
                s.pop();
            }
            s.pop();
        }
    }
    while (!s.empty())
    {
        suffix += s.top();
        s.pop();
    }
    return suffix;
}

/*
** eval
** 计算表达式
** row     : 每行变元对应的值的序列
** suffix  : 后缀表示的表达式
** exp_elem: 存储表达式变元信息
*/
int eval(const string& row, const string& suffix, const set<char>& exp_elem)
{
    char temp = 0;
    int p, q, ret;
    stack<char> s;
    map<char, int> m;
    string::size_type i;

    set<char>::const_iterator pos;
    for (pos = exp_elem.begin(), i = 0; pos != exp_elem.end(); ++pos, ++i)
    {
        m.insert(pair<char, int>(*pos, row[i] - '0'));
    }

    p = q = 0;
    for (i = 0; i != suffix.size(); ++i)
    {
        if (isupper(suffix[i]))
            s.push(m[suffix[i]]);
        else if (isoperator(suffix[i]))
        {
            q = s.top();
            s.pop();

            if (suffix[i] != '!' && !s.empty())
            {
                p = s.top();
                s.pop();
            }

            switch (suffix[i])
            {
            case '!':
                temp = !q;
                break;
            case '&':
                temp = p && q;
                break;
            case '|':
                temp = p || q;
                break;
            case '>':
                temp = !p || q;
                break;
            case '#':
                temp = (!p || q) && (!q || p);
                break;
            }
            /* 把运算的结果再压回堆栈中 */
            s.push(temp);
        }
    }
    ret = s.top();
    s.pop();
    return ret;
}

/*
** 打印真值表
*/
vector<char> print_table(const string& exp)
{
    int cols, rows, temp;
    string row, suffix;
    set<char> exp_elem;

    exp_elem = get_expinfo(exp);
    for (set<char>::iterator pos = exp_elem.begin(); pos != exp_elem.end(); ++pos)
    {
        cout << '\t' << *pos;
    }
    cout << '\t' << exp << endl;

    suffix = infix_to_suffix(exp);

    cols = exp_elem.size();
    rows = static_cast<int>(pow(2.0, cols));

    bitset<26> bs;
    vector<char> result;
    for (int i = 0; i < rows; ++i)
    {
        bs = i;
#if (_MSC_VER == 1200)  //VC6
        row = bs.to_string();
#else
        row = bs.to_string<char, char_traits<char>, allocator<char> >();
#endif
        row.erase(0, 26 - cols);
        for (int j = 0; j < cols; ++j)
        {
            cout << '\t' << row[j];
        }
        temp = eval(row, suffix, exp_elem);
        result.push_back(temp);
        cout << '\t' << temp << endl;
    }
    return result;
}

/*
** 判断是否是重言式(永真式)
*/
int is_tautology(const vector<char>& result)
{
    return result.end() == find(result.begin(), result.end(), 0);
}

/*
** 判断是否是矛盾式(永假式)
*/
int is_contradiction(const vector<char>& result)
{
    return result.end() == find(result.begin(), result.end(), 1);
}

/*
** 主合取范式 或与表达式 大项合取而成
*/
void print_cnf(const vector<char>& result, const set<char>& exp_elem)
{
    //永真式没有合取范式
    if (is_tautology(result))
        return;

    vector<char> elem;
    copy(exp_elem.begin(), exp_elem.end(), back_inserter(elem));

    size_t i, j;
    vector<int> v;
    for (i = 0; i != result.size(); ++i)
    {
        if (result[i] == 0)
            v.push_back(i);
    }

    cout << "主合取范式\n" << "M(";
    for (i = 0; i != v.size(); ++i)
    {
        if (i < v.size() - 1)
            cout << v[i] << ", ";
        else
            cout << v[i];
    }
    cout << ')' << endl;

    bitset<26> bs;
    string row;
    for (i = 0; i != v.size(); ++i)
    {
        cout << '(';
        bs = v[i];
#if (_MSC_VER == 1200)  //VC6
        row = bs.to_string();
#else
        row = bs.to_string<char, char_traits<char>, allocator<char> >();
#endif
        row.erase(0, 26 - elem.size());
        for (j = 0; j != elem.size(); ++j)
        {
            if (row[j] == '1')
                cout << "┐" << elem[j];
            else
                cout << elem[j];

            if (j < elem.size() - 1)
                cout << "∨";
        }
        if (i < v.size() - 1)
            cout << ")∧";
        else
            cout << ')' << endl;
    }
}

/*
** 主析取范式 与或表达式 小项析取而成
*/
void print_dnf(const vector<char>& result, const set<char>& exp_elem)
{
    //永假式没有析取范式
    if (is_contradiction(result))
        return;

    vector<char> elem;
    copy(exp_elem.begin(), exp_elem.end(), back_inserter(elem));

    size_t i, j;
    vector<int> v;
    for (i = 0; i != result.size(); ++i)
    {
        if (result[i] == 1)
            v.push_back(i);
    }

    cout << "主析取范式\n" << "M(";
    for (i = 0; i != v.size(); ++i)
    {
        if (i < v.size() - 1)
            cout << v[i] << ", ";
        else
            cout << v[i];
    }
    cout << ')' << endl;

    bitset<26> bs;
    string row;
    for (i = 0; i != v.size(); ++i)
    {
        cout << '(';
        bs = v[i];
#if (_MSC_VER == 1200)  //VC6
        row = bs.to_string();
#else
        row = bs.to_string<char, char_traits<char>, allocator<char> >();
#endif
        row.erase(0, 26 - elem.size());
        for (j = 0; j != elem.size(); ++j)
        {
            if (row[j] == '1')
                cout << elem[j];
            else
                cout << "┐" << elem[j];

            if (j < elem.size() - 1)
                cout << "∧";
        }
        if (i < v.size() - 1)
            cout << ")∨";
        else
            cout << ')' << endl;
    }
}

void cls()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause()
{
#ifdef _WIN32
    system("pause");
#else
    system("read -p \"Press any key to continue...\" tmp");
#endif
}

void instruction()
{
    cout << "||====离散真值表生成器====||\n"
         << "||     '!' 否定联结词     ||\n"
         << "||     '&' 合取联结词     ||\n"
         << "||     '|' 析取联结词     ||\n"
         << "||     '>' 蕴含联结词     ||\n"
         << "||     '#' 等值联结词     ||\n"
         << "||     'q' 退出程序       ||\n"
         << "||========================||\n"
         << "输入表达式:";
}

int main()
{
    string exp;
    set<char> exp_elem;
    vector<char> result;

#ifdef _WIN32
    system("title 离散数学真值表");
#endif

    while (1)
    {
        cls();
        instruction();
        cin >> exp;

        if (exp == "q")
            break;
        if (!is_valid_expression(exp))
        {
            pause();
            continue;
        }
        exp_elem = get_expinfo(exp);
        result = print_table(exp);
        print_cnf(result, exp_elem);
        print_dnf(result, exp_elem);
        pause();
    }
    return 0;
}