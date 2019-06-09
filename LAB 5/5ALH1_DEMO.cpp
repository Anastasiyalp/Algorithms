#include <iostream>
#include <cstring>
#include <vector>

#include <windows.h>

using namespace std;
// ������ ��������
const int k = 60;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

struct bohr_vrtx
{
    int next_vrtx[k];   //next_vrtx[i] � ����� �������, � ������� �� ������ �� ������� � ������� i � ��������
    int pat_num;        //����� ������-�������, ������������� ���� ��������
    int suff_link;      //suff_link - ���������� ������
    int auto_move[k];   //auto_move - ����������� �������� ��������
    int par;            //par - �������-���� � ������
    int64_t suff_flink; //suff_flink - "�������" ���. ������
    bool flag;          //flag � ���, ����������� �� ��, �������� �� ���� ������� �������� �������
    char symb;          //������ �� ����� �� par � ���� �������
    int count_arc;      //���������� �������� ��� �� �������
};

class Bohr
{
    vector<bohr_vrtx> bohr;
    vector<string> pattern;
    vector<pair<int, int>> remove_text;

    //������� ���������� ������� � ���
    bohr_vrtx make_bohr_vrtx(int p, char c) //�������� ����� ���� � ������ �� ����� � ����
    {
        bohr_vrtx v; //(255)=(2^8-1)=(��� ������� � ������ ����� ������)=(-1 � �������������� ���� ������ 4-�������� ����� int)
        memset(v.next_vrtx, 255, sizeof(v.next_vrtx));
        memset(v.auto_move, 255, sizeof(v.auto_move));
        v.flag = false;   //�� ���������: ������� - �� ������� ������
        v.suff_link = -1; //���������� - ���. ������ ���
        v.par = p;
        v.symb = c;
        v.suff_flink = -1;
        v.count_arc = 0;
        return v;
    }

  public:
    Bohr()
    {
        //��������� ������������ ������� - ������
        bohr.push_back(make_bohr_vrtx(0, '$'));
    }
    ~Bohr() = default;

    //���������� ������ � ���
    void add_string_to_bohr(const string &s)
    {
        int num = 0; //�������� � �����
        for (int i = 0; i < s.length(); i++)
        {
            cout << '\t' << s[i] << " - ";
            int ch = s[i] - 'A';               //�������� ����� � �������� ��� ������� ������
            if (bohr[num].next_vrtx[ch] == -1) //-1 - ������� ���������� �����
            {
                cout << "������� � ���� ���. �������� ����� ��� ����." << endl;
                bohr[num].count_arc++;
                bohr.push_back(make_bohr_vrtx(num, ch));   //�������� �������
                bohr[num].next_vrtx[ch] = bohr.size() - 1; //������������ �����
            }
            else
                cout << "������ ��� � ����. ��������� � ����." << endl;
            num = bohr[num].next_vrtx[ch]; //������� � ��������� �������
        }
        cout << "\t������� ����� ������." << endl;
        bohr[num].flag = true; //������� ����� ��������
        pattern.push_back(s);
        bohr[num].pat_num = pattern.size() - 1;
    }

  private:
    //������� �� ���. ������
    int get_suff_link(int v, int count_rec)
    {
        for(int i = 0; i < count_rec; i++)
            cout << '\t';
        cout <<"\t��� " << (v ? (char)(bohr[v].symb +'A') : '$') << " ";
        if (bohr[v].suff_link == -1)        //���� ��� �� �������
        {
            cout << "����. ������ �� ����������." << endl;
            for(int i = 0; i < count_rec; i++)
                cout << '\t';
            if (v == 0 || bohr[v].par == 0) //���� v - ������ ��� ������ v - ������
            {
                cout << "\t�� � �����. ���� �� ����. ������������� ����. ������ ����� � ������." << endl;
                bohr[v].suff_link = 0;
            }else
            {
                cout << "\t�������� ��. �������� � ��������:" << endl;
                bohr[v].suff_link = get_auto_move(get_suff_link(bohr[v].par, count_rec + 1), bohr[v].symb);
            }
        }else
            cout << "����. ������ ��� ���� �������. ��� ����� � "<< (bohr[v].suff_link ? (char)(bohr[bohr[v].suff_link].symb +'A') : '$') << endl;
        return bohr[v].suff_link;
    }
    //������� ��������
    int get_auto_move(int v, char ch)
    {
        if (bohr[v].auto_move[ch] == -1)                                     //���� ��� ������ ������ ������� �������� ��� �� ���������, �� ��������� ���
        {
            cout << "\t������� �������� �� ��� ���������. ��������� ���:" << endl;
            if (bohr[v].next_vrtx[ch] != -1)                                 //���� �� ������� v ���� ���� � ch
            {
                cout << "\t���� ������� ���� (" << (v ? (char)(bohr[v].symb +'A') : '$') <<", " << (char)(ch+'A') << ")" << endl;
                cout << "\t������ ������� ���������." << endl;
                bohr[v].auto_move[ch] = bohr[v].next_vrtx[ch];               //��������� ������� �������� � ���
            }else if (v == 0)                                                 //���� ������ � ������
            {
                cout << "\t���� (" << (v ? (char)(bohr[v].symb +'A') : '$') <<", " << (char)(ch+'A') << ") �� �������" << endl;
                cout << "\t������ � �����. ������ ������� ���������." << endl;
                bohr[v].auto_move[ch] = 0;                                   //��������� ������� �������� � ������
            }else                                                             //�����
            {
                cout << "\t���� (" << (v ? (char)(bohr[v].symb +'A') : '$') <<", " << (char)(ch+'A') << ") �� �������." << endl;
                cout << "\t��������� �� ���������� ������." << endl;
                bohr[v].auto_move[ch] = get_auto_move(get_suff_link(v, 0), ch); //��������� ������� �������� ���������� � ������������ � ��������� ��� ������� � ������� ����� ����. ������ ������� �������
            }
        }else
            cout << "������� �������� ��� ���������.";
        return bohr[v].auto_move[ch];
    }
    // ���������� "�������" ���. ������
    int get_suff_flink(int v, int count_rec)
    {
        if (bohr[v].suff_flink == -1)
        {
            int u = get_suff_link(v, count_rec); //������� �� ����. ������
            if (u == 0)               //���� v - ������, ���� ����. ������ v ��������� �� ������
                bohr[v].suff_flink = 0;
            else
                bohr[v].suff_flink = (bohr[u].flag) ? u : get_suff_flink(u, count_rec + 1); // � ������ ������������ ����� ��������, ����������� �����
        }
        return bohr[v].suff_flink;
    }
    // �������� ��������� �� ���������� � ����
    void check(int v, int i)
    {
        for (int u = v; u != 0; u = get_suff_flink(u, 0)) //�������� �� ����. �������
        {
            if (bohr[u].flag)
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                cout << "������� ���������: " << pattern[bohr[u].pat_num] << endl;
                cout << i - pattern[bohr[u].pat_num].length() + 1 << " " << bohr[u].pat_num + 1 << endl;
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN |FOREGROUND_RED);
                remove_text.push_back({i - pattern[bohr[u].pat_num].length(), pattern[bohr[u].pat_num].size()}); //���������� � ������ ��� ����������� ��������� ������
            }
        }
    }

  public:
    // ���������� ���� ��������� ��������� � �����
    void find_all_pos(const string &s)
    {
        int u = 0;
        for (int i = 0; i < s.length(); i++)
        {
            cout << endl << i+1<< ") "<< s[i] << " - ";
            u = get_auto_move(u, s[i] - 'A'); //������� ��������
            cout << endl << "\t�������� ��������� �� ���������� � ����:" << endl;
            check(u, i + 1);                  // �������� ��������� �� ���������� � ����
            print_bohr(u);
        }
    }
    // ���������� �������������� ��������� ��� � ����
    int max_arc()
    {
        int i_max_count_arc = 0;
        for (int i = 0; i < bohr.size(); i++)
            if (bohr[i_max_count_arc].count_arc < bohr[i].count_arc)
                i_max_count_arc = i;
        return bohr[i_max_count_arc].count_arc;
    }
    // ������� �� ������ ��� ��������� ��������
    string text_without_patterns(const string &s)
    {
        string final_text;
        bool remove_ind[s.length()]; //������ ������� �� �������� ����� �� ������
        memset(remove_ind, true, s.length());
        for (int i = 0; i < remove_text.size(); i++) //������� �������� �������� false
            memset(remove_ind + remove_text[i].first, false, remove_text[i].second);
        for (int i = 0; i < s.length(); i++)
            if (remove_ind[i]) //��� ���� �� ���������� ����: �������� � ������
                final_text += s[i];
        return final_text;
    }
    //���������� ����
    void print_bohr(int cur_vrtx)
    {
        if(cur_vrtx != -1)
            if(bohr[cur_vrtx].suff_link == 0)
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << bohr[0].symb << endl;
        if(cur_vrtx!= -1)
            if(bohr[cur_vrtx].suff_link == 0)
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN |FOREGROUND_RED);
        print_next(0, 0, cur_vrtx);
    }

  private:
    void print_next(int n, int count_tab, int cur_vrtx) 
    {
        int next;
        int next_count_tab = 0;
        for (int i = 0; i < k; i++)
        {
            next = bohr[n].next_vrtx[i];
            if (next != -1)
            {
                cout << " - ";
                if(next == cur_vrtx)
                    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                if(cur_vrtx != -1)
                    if(bohr[cur_vrtx].suff_link == next)
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                cout << (char)(bohr[next].symb + 'A');
                if(next == cur_vrtx)
                    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN |FOREGROUND_RED);
                if(cur_vrtx!= -1)
                    if(bohr[cur_vrtx].suff_link == next)
                        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN |FOREGROUND_RED);
                if (bohr[next].flag)
                    cout << "(" << bohr[next].pat_num + 1 << ")";
                print_next(next, count_tab + 1, cur_vrtx);
                next_count_tab++;
                if (bohr[next].count_arc == 0)
                    cout << endl;
                if (next_count_tab != bohr[n].count_arc)
                    for (int p = 0; p < count_tab; p++)
                        cout << "   ^";
            }
        }
    };

  public:
    //���������� ���������
    void print_patterns()
    {
        for (int i = 0; i < pattern.size(); i++)
            cout << i + 1 << ") " << pattern[i] << endl;
    }
};

int main()
{
    setlocale(LC_ALL, "rus");
    string T, pattern;
    int n;
    cout << "������� ����� � ���������� ���������:" << endl;
    cin >> T >> n;
    // ������������� ����
    cout << "������������� ����:" << endl;
    Bohr bohr;
    cout << "���������� ��������� � ���:" << endl;
    for (int i = 0; i < n; i++)
    {
        cout << i + 1 << ") ";
        cin >> pattern;
        // ���������� ��������� � ���
        bohr.add_string_to_bohr(pattern);
        bohr.print_bohr(-1);
    }
    cout << "�������� ������������ ���� ��� ������ ���������:" << endl;
    bohr.print_patterns();
    bohr.print_bohr(-1);
    cout << endl << endl << "���������� ���� ��������� � ������ " << T << ":" << endl;
    bohr.find_all_pos(T);
    cout << "max_arc : " << bohr.max_arc() << endl;
    cout << "Text without patterns: " << bohr.text_without_patterns(T) << endl;
}