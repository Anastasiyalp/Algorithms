#include <iostream>
#include <cstring>
#include <vector>

using namespace std;
// ������ ��������
const int k = 90;

struct bohr_vrtx
{
    int next_vrtx[k];    //next_vrtx[i] � ����� �������, � ������� �� ������ �� ������� � ������� i � ��������
    int pat_num;         //����� ������-�������, ������������� ���� ��������
    int suff_link;       //suff_link - ���������� ������
    int auto_move[k];    //auto_move - ����������� �������� �������� 
    int par;             //par - �������-���� � ������
    int64_t suff_flink;  //suff_flink - "�������" ���. ������
    bool flag;           //flag � ���, ����������� �� ��, �������� �� ���� ������� �������� �������
    char symb;           //������ �� ����� �� par � ���� ������� 
    int count_arc;       //���������� �������� ��� �� �������
};

class Bohr
{
    vector<bohr_vrtx> bohr;
    vector<string> pattern;
    vector<pair<int, int>> remove_text;

  public:
    char joker;

  private:
    //������� ���������� ������� � ���
    bohr_vrtx make_bohr_vrtx(int p, char c) //�������� ����� ���� � ������ �� ����� � ����
    {
        bohr_vrtx v; //(255)=(2^8-1)=(��� ������� � ������ ����� ������)=(-1 � �������������� ���� ������ 4-�������� ����� int)
        memset(v.next_vrtx, 255, sizeof(v.next_vrtx));
        memset(v.auto_move, 255, sizeof(v.auto_move));
        v.flag = false; //�� ���������: ������� - �� ������� ������
        v.suff_link = -1;  //���������� - ���. ������ ���
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

    //���������� ������ � ���
    void add_string_to_bohr(const string &s)
    {
        int num = 0; //�������� � �����
        for (int i = 0; i < s.length(); i++)
        {
            int ch = s[i] - ' '; //�������� ����� � �������� ��� ������� ������
            if (bohr[num].next_vrtx[ch] == -1) //-1 - ������� ���������� �����
            {
                bohr[num].count_arc++;
                bohr.push_back(make_bohr_vrtx(num, ch)); //�������� �������
                bohr[num].next_vrtx[ch] = bohr.size() - 1; //������������ �����
            }
            num = bohr[num].next_vrtx[ch]; //������� � ��������� �������
        }
        bohr[num].flag = true; //������� ����� ��������
        pattern.push_back(s);
        bohr[num].pat_num = pattern.size() - 1;
    }

  private:
    //������� �� ���. ������
    int get_suff_link(int v)
    {
        if (bohr[v].suff_link == -1)
            if (bohr[bohr[v].par].symb == joker)
                bohr[v].suff_link = bohr[v].par;
            else
            if (v == 0 || bohr[v].par == 0)
                bohr[v].suff_link = 0;
            else
                bohr[v].suff_link = get_auto_move(get_suff_link(bohr[v].par), bohr[v].symb);
        return bohr[v].suff_link;
    }

    //������� ��������
    int get_auto_move(int v, char ch)
    {
        if (bohr[v].auto_move[ch] == -1)
            if (bohr[v].next_vrtx[ch] != -1)
                bohr[v].auto_move[ch] = bohr[v].next_vrtx[ch];
            else if (bohr[v].next_vrtx[joker] != -1)
                bohr[v].auto_move[ch] = bohr[v].next_vrtx[joker];
            else if (v == 0)
                bohr[v].auto_move[ch] = 0;
            else
                bohr[v].auto_move[ch] = get_auto_move(get_suff_link(v), ch);
        return bohr[v].auto_move[ch];
    }
    // ���������� "�������" ���. ������
    int get_suff_flink(int v)
    {
        if (bohr[v].suff_flink == -1)
        {
            int u = get_suff_link(v);
            if (u == 0)
                bohr[v].suff_flink = 0;
            else
                bohr[v].suff_flink = (bohr[u].flag) ? u : get_suff_flink(u);
        }
        return bohr[v].suff_flink;
    }

    // �������� ��������� �� ���������� � ���� 
    void check(int v, int i)
    {
        for (int u = v; u != 0; u = get_suff_flink(u))
        {
            if (bohr[u].flag)
            {
                cout << i - pattern[bohr[u].pat_num].length() + 1 << endl;
                remove_text.push_back({i - pattern[bohr[u].pat_num].length(), pattern[bohr[u].pat_num].size()});
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
            u = get_auto_move(u, s[i] - ' ');
            check(u, i + 1);
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
        bool remove_ind[s.length()];
        memset(remove_ind, true, s.length());
        for (int i = 0; i < remove_text.size(); i++)
            memset(remove_ind + remove_text[i].first, false, remove_text[i].second);
        for (int i = 0; i < s.length(); i++)
            if (remove_ind[i])
                final_text += s[i];
        return final_text;
    }
};

int main()
{
    string T, pattern;
    char joker;
    cin >> T >> pattern >> joker;
    joker -= ' ';
    // ������������� ����
    Bohr bohr;
    //������������� �������
    bohr.joker = joker;
    bohr.add_string_to_bohr(pattern);
    // ���������� ���� ��������� � ������ T
    bohr.find_all_pos(T);
    cout << "max_arc : " << bohr.max_arc() << endl;
    cout << "Text without patterns: " << bohr.text_without_patterns(T) << endl;
}