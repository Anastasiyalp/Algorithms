#include <iostream>
#include <cstring>
#include <vector>

using namespace std;
// Размер алфавита
const int k = 60;

struct bohr_vrtx
{
    int next_vrtx[k];    //next_vrtx[i] — номер вершины, в которую мы придем по символу с номером i в алфавите
    int pat_num;         //номер строки-образца, обозначаемого этой вершиной
    int suff_link;       //suff_link - суффиксная ссылка
    int auto_move[k];    //auto_move - запоминание перехода автомата 
    int par;             //par - вершина-отец в дереве
    int64_t suff_flink;  //suff_flink - "хорошая" суф. ссылка
    bool flag;           //flag — бит, указывающий на то, является ли наша вершина исходной строкой
    char symb;           //символ на ребре от par к этой вершине 
    int count_arc;       //количество исхдящих дуг из вершины
};

class Bohr
{
    vector<bohr_vrtx> bohr;
    vector<string> pattern;
    vector<pair<int, int>> remove_text;
    
    //Функция добваления вершины в бор
    bohr_vrtx make_bohr_vrtx(int p, char c) //передаем номер отца и символ на ребре в боре
    {
        bohr_vrtx v; //(255)=(2^8-1)=(все единицы в каждом байте памяти)=(-1 в дополнительном коде целого 4-байтного числа int)
        memset(v.next_vrtx, 255, sizeof(v.next_vrtx));
        memset(v.auto_move, 255, sizeof(v.auto_move));
        v.flag = false; //По умолчанию: вершина - не сходная строка
        v.suff_link = -1;  //изначально - суф. ссылки нет
        v.par = p;
        v.symb = c;
        v.suff_flink = -1;
        v.count_arc = 0;
        return v;
    }
public:
    Bohr()
    {
        //добавляем единственную вершину - корень
        bohr.push_back(make_bohr_vrtx(0, '$'));
    }
    ~Bohr() = default;

    //Добавление строки в бор
    void add_string_to_bohr(const string &s)
    {
        int num = 0; //начинаем с корня
        for (int i = 0; i < s.length(); i++)
        {
            int ch = s[i] - 'A'; //получаем номер в алфавите для символа строки
            if (bohr[num].next_vrtx[ch] == -1) //-1 - признак отсутствия ребра
            {
                bohr[num].count_arc++;
                bohr.push_back(make_bohr_vrtx(num, ch)); //Создание вершины
                bohr[num].next_vrtx[ch] = bohr.size() - 1; //Установление ребра
            }
            num = bohr[num].next_vrtx[ch]; //Переход в следующую вершину
        }
        bohr[num].flag = true; //Пометка конца паттерна
        pattern.push_back(s);
        bohr[num].pat_num = pattern.size() - 1;
    }
private:

    //Переход по суф. ссылке
    int get_suff_link(int v) 
    {
        if (bohr[v].suff_link == -1) //если еще не считали
            if (v == 0 || bohr[v].par == 0) //если v - корень или предок v - корень
                bohr[v].suff_link = 0;
            else
                bohr[v].suff_link = get_auto_move(get_suff_link(bohr[v].par), bohr[v].symb);
        return bohr[v].suff_link;
    }
    //Функция автомата
    int get_auto_move(int v, char ch)
    {
        if (bohr[v].auto_move[ch] == -1) //Если для нашего случая переход автомата еще не определен, то определим его
            if (bohr[v].next_vrtx[ch] != -1) //Если из вершины v есть путь в ch
                bohr[v].auto_move[ch] = bohr[v].next_vrtx[ch]; //Установим автомат перехода в нее
            else if (v == 0) //Если пришли в корень
                bohr[v].auto_move[ch] = 0; //Установим автомат перехода в корень
            else //Иначе
                bohr[v].auto_move[ch] = get_auto_move(get_suff_link(v), ch);  //Установим автомат перехода рекурсивно в соответствии с автоматом для вершины в которую ведет суфф. ссылка текушей вершины
        return bohr[v].auto_move[ch];
    }
    // Нахождение "хорошей" суф. ссылки
    int get_suff_flink(int v)
    {
        if (bohr[v].suff_flink == -1)
        {
            int u = get_suff_link(v); //Переход по суфф. ссылке
            if (u == 0) //либо v - корень, либо суфф. ссылка v указывает на корень 
                bohr[v].suff_flink = 0;
            else
                bohr[v].suff_flink = (bohr[u].flag) ? u : get_suff_flink(u); // В случае ненахождения конца паттерна, рекурсивный поиск
        }
        return bohr[v].suff_flink;
    }
    // Проверка подстроки на нахождение в боре 
    void check(int v, int i)
    {
        for (int u = v; u != 0; u = get_suff_flink(u)) //Переходы по суфф. ссылкам
        {
            if (bohr[u].flag)
            {
                cout << i - pattern[bohr[u].pat_num].length() + 1 << " " << bohr[u].pat_num + 1 << endl;
                remove_text.push_back({i - pattern[bohr[u].pat_num].length(), pattern[bohr[u].pat_num].size()}); //Добавление в вектор для последующей обработки текста
            }
        }
    }
public:
    // Нахождение всех вхождений паттернов в текст
    void find_all_pos(const string &s)
    {
        int u = 0;
        for (int i = 0; i < s.length(); i++)
        {
            u = get_auto_move(u, s[i] - 'A'); //Переход автомата
            check(u, i + 1); // Проверка подстроки на нахождение в боре 
        }
    }
    // Нахождение максиматльного кличества дуг в боре
    int max_arc()
    {
        int i_max_count_arc = 0;
        for (int i = 0; i < bohr.size(); i++)
            if (bohr[i_max_count_arc].count_arc < bohr[i].count_arc)
                i_max_count_arc = i;
        return bohr[i_max_count_arc].count_arc;
    }
    // Удаляет из текста все найденные паттерны 
    string text_without_patterns(const string &s)
    {
        string final_text;
        bool remove_ind[s.length()];    //Массив пометок об удалении буквы из строки
        memset(remove_ind, true, s.length());
        for (int i = 0; i < remove_text.size(); i++) //Область паттерна помечена false
            memset(remove_ind + remove_text[i].first, false, remove_text[i].second);
        for (int i = 0; i < s.length(); i++)
            if (remove_ind[i]) //Для всех не помеченных букв: добавить к строке
                final_text += s[i];
        return final_text;
    }
};

int main()
{
    string T, pattern;
    int n;
    cin >> T >> n;
    // Инициализация бора
    Bohr bohr;
    for (int i = 0; i < n; i++)
    {
        cin >> pattern;
        // Добавление паттернов в бор
        bohr.add_string_to_bohr(pattern);
    }
    // Нахождение всех паттернов в строке T
    bohr.find_all_pos(T);
    cout << "max_arc : " << bohr.max_arc() << endl;
    cout << "Text without patterns: " << bohr.text_without_patterns(T) << endl;
}