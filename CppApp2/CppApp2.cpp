#include <iostream>
#include <stdio.h>
#include <locale.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <queue> 

using namespace std;

class TGF;
class Vertex;
class Edge;
class EdgeList_Graph;

class TGF {
public:
	string path;
	int flag, isopen;
	TGF() {
		flag = 0;
		isopen = 0;
	}
	void OpenFile();
	friend EdgeList_Graph;
	friend void CountVR(TGF* a, EdgeList_Graph* b);
	friend void GetVR(TGF* a, Vertex* vert, Edge* edge);
};

class Vertex {
protected:
	int index, first;
	string name;
public:
	Vertex() {
		first = 0;
	}
	friend int FIRST(int n, Vertex* vert);
	friend EdgeList_Graph;
	friend void GetVR(TGF* a, Vertex* vert, Edge* edge);
	friend int NEXT(int n, int v, int i, Vertex* vert, Edge* edge);

};

class Edge {
protected:
	int v1, v2, weight;
public:
	friend EdgeList_Graph;
	friend void GetVR(TGF* a, Vertex* vert, Edge* edge);
	friend int NEXT(int n, int v, int i, Vertex* vert, Edge* edge);
	friend int BFS(int n, int a, int r, Edge* edge);
	friend int Smezh(int n, int v, Edge* edge);
};

class EdgeList_Graph {
public:
	Vertex* listV;
	Edge* listR;
	int countV = 0, countR = 0;

	~EdgeList_Graph() {
		delete[] listV;
		delete[] listR;
	}

	Vertex* getV() {
		return listV;
	}
	Edge* getR() {
		return listR;
	}


	void List();
	void PrintV();
	void PrintR();
	int ChangeGraph();
	void AddToVertex();
	int AddToEdge();
	int DeleteVertex();
	int DeleteEdge();
	int ChangeVertex();
	int ChangeEdge();
	int VERTEX(int i, int v);
	friend void CountVR(TGF* a, EdgeList_Graph* b);
	friend void GetVR(TGF* a, Vertex* vert, Edge* edge);
};


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

int SearchRadius(EdgeList_Graph* a, Edge* edge) {
	int n = (*a).countV;
	int r = (*a).countR;
	int min = n * n;
	vector<int> b(n);
	for (int i = 0; i < n; i++) {
		b[i] = BFS(n, i, r, edge);
		if (b[i] < min) min = b[i];
	}
	return(min);
}

int BFS(int n, int a, int r, Edge* edge) {
	int to, max = 1;
	queue<int> q;
	vector<int> used(n, 0);
	q.push(a);
	used[a] = 1;
	while (!q.empty()) {
		int v = q.front();
		q.pop();
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < r; j++) {
				if (v == (edge[i].v1 - 1)) {
					to = edge[i].v2 - 1;
					if (!used[to]) {
						used[to] = 1;
						q.push(to);
					}
				}
				if (v == (edge[i].v2 - 1)) {
					to = edge[i].v1 - 1;
					if (!used[to]) {
						used[to] = 1;
						q.push(to);
					}
				}
			}
		}
		max++;
	}
	return(max);
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


void TGF::OpenFile() {
	if (flag == 0) {
		path = "";
		cout << "Введите название файла: ";
		cin >> path;
	}
	else {
		cout << "Файл уже открыт" << endl;
	}
}

void EdgeList_Graph::List() {
	if (countV != 0)
		listV = new Vertex[countV];
	else cout << "\nОшибка! Отсутствуют вершины графа!" << endl;

	if (countR != 0)
		listR = new Edge[countR];
	else cout << "\nОшибка! Отсутствуют рёбра графа!" << endl;
}

void CountVR(TGF* a, EdgeList_Graph* b) {
	ifstream potok;
	potok.open(a->path);
	if (!potok.is_open()) {
		cout << "Ошибка прочтения файла" << endl << endl;
	}
	else {
		cout << endl << "Файл был считан" << endl;
		a->flag++;
		a->isopen++;
		int count = 0;
		string strng = "";

		while (strng.find("#")) {
			getline(potok, strng);
			(*b).countV++;
		}
		while (!potok.eof()) {
			getline(potok, strng);
			(*b).countR++;
		}
		(*b).countV--;
		(*b).countR--;
	}

	potok.close();
}

void GetVR(TGF* a, Vertex* vert, Edge* edge) {
	ifstream potok;
	potok.open(a->path);

	if (!potok.is_open()) {
		cout << "Ошибка прочтения файла" << endl << endl;
	}
	else {
		int count = 0, count2 = 0, flag = 0;
		string strng = "";

		while (strng.find("#")) {
			flag++;
			getline(potok, strng);
			if (strng.length() < 2) break;
			if (strng[strng.length() - 1] == ' ')
				strng += 'NN';
			strng += '.';

			vector <string> duo;
			string chartog;

			for (int i = 0; i < strng.length(); i++) {

				if (strng[i] == ' ' || strng[i] == '.')
				{
					duo.push_back(chartog);
					chartog.clear();
				}
				else
					chartog += strng[i];
			}
			vert[count].index = atoi(duo[0].c_str());
			vert[count].name = duo[1];
			if (flag == 1) vert[count].first = 1;
			count++;

		}
		count = 0;
		strng = "";
		while (!potok.eof()) {
			getline(potok, strng);
			if (strng.length() < 3) break;
			strng += '.';
			vector <string> trio;
			string chartog;

			for (int i = 0; i < strng.length(); i++) {
				if (strng[i] == ' ' || strng[i] == '.')
				{
					if (strng[i] == ' ') count2++;
					trio.push_back(chartog);
					chartog.clear();
				}
				else
					chartog += strng[i];
			}
			if (count2 == 2) {
				edge[count].v1 = atoi(trio[0].c_str());
				edge[count].v2 = atoi(trio[1].c_str());
				edge[count].weight = atoi(trio[2].c_str());
			}
			else {
				edge[count].v1 = atoi(trio[0].c_str());
				edge[count].v2 = atoi(trio[1].c_str());
				edge[count].weight = 1;
			}
			count++;
			count2 = 0;
		}
	}
	potok.close();
}

void EdgeList_Graph::PrintV() {
	int count = countV;
	cout << endl << endl;
	for (int i = 0; i < count; i++) {
		if (i == 0) cout << "INDEX\t" << "NAME" << endl;
		cout << listV[i].index << "\t " << listV[i].name << endl;
	}
	cout << endl << endl;
}

void EdgeList_Graph::PrintR() {
	int count = countR;
	cout << endl << endl;
	for (int i = 0; i < count; i++) {
		if (i == 0) cout << "INDEX\t" << "VERTEX-1\t" << "VERTEX-2\t" << "WEIGHT" << endl;
		cout << i + 1 << "\t     " << listR[i].v1 << "\t\t   " << listR[i].v2 << "\t\t " << listR[i].weight << endl;
	}
	cout << endl << endl;
}

int EdgeList_Graph::ChangeGraph() {
	int i;
	do {
		cout << endl << endl << endl;
		cout << "4.1: Добавить вершину" << endl;
		cout << "4.2: Удалить вершину" << endl;
		cout << "4.3: Изменить вершину" << endl;
		cout << "4.4: Добавить ребро" << endl;
		cout << "4.5: Удалить ребро" << endl;
		cout << "4.6: Изменить ребро" << endl << endl;

		cout << "4.0: Вернуться в предыдущее меню" << endl;

		cout << endl << "\t# ";
		cin >> i;
		cout << endl;
		switch (i) {
		case 1:
			AddToVertex();
			break;
		case 2:
			DeleteVertex();
			break;
		case 3:
			ChangeVertex();
			break;
		case 4:
			AddToEdge();
			break;
		case 5:
			DeleteEdge();
			break;
		case 6:
			ChangeEdge();
			break;
		case 0:
			return 0;
			break;
		default:
			cout << endl << "Выберите один из вариантов" << endl;
			break;
		}
	} while (i != 0);
}

void EdgeList_Graph::AddToVertex() {
	Vertex* nlist = new Vertex[countV + 1];

	for (int i = 0; i < countV; i++) {
		nlist[i].index = listV[i].index;
		nlist[i].name = listV[i].name;
	}
	nlist[countV].index = countV + 1;
	cout << "\tВведите имя новой вершины: ";
	cin >> nlist[countV].name;
	delete[] listV;
	listV = nlist;
	countV++;
}

int EdgeList_Graph::AddToEdge() {
	Edge* nlist = new Edge[countR + 1];
	int foo = 0;
	for (int i = 0; i < countR; i++) {
		nlist[i].v1 = listR[i].v1;
		nlist[i].v2 = listR[i].v2;
		nlist[i].weight = listR[i].weight;
	}
	cout << "Введите имя вершины из которой нужно проложить ребро: ";
	cin >> nlist[countR].v1;
	cout << endl << "Введите имя вершины в которую нужно проложить ребро: ";
	cin >> nlist[countR].v2;
	for (int i = 0; i < countR; i++) {
		if ((listR[i].v1 == nlist[countR].v1) && (listR[i].v2 == nlist[countR].v2)) {
			cout << "Данное ребро уже существует" << endl << endl;
			return 0;
		}
		if ((listR[i].v2 == nlist[countR].v1) && (listR[i].v1 == nlist[countR].v2)) {
			nlist[countR].weight = listR[i].weight;
			foo++;
		}
	}
	delete[] listR;
	listR = nlist;
	countR++;
	return 0;
}

int EdgeList_Graph::DeleteVertex() {
	int index;
	cout << "Введите индекс вершины, которую нужно удалить: ";
	cin >> index;
	Vertex* nlist = new Vertex[countV - 1];
	if (index == 1) {
		for (int i = 1; i < countV; i++) {
			nlist[i].index = listV[i].index;
			nlist[i].name = listV[i].name;
		}
	}
	if (index == (countV)) {
		for (int i = 0; i < countV - 1; i++) {
			nlist[i].index = listV[i].index;
			nlist[i].name = listV[i].name;
		}
	}
	if (index < countV && index>1) {
		for (int i = 0; i < index; i++) {
			nlist[i].index = listV[i].index;
			nlist[i].name = listV[i].name;
		}
		for (int i = index + 1; i < countV; i++) {
			nlist[i].index = listV[i].index;
			nlist[i].name = listV[i].name;
		}
	}
	if (index<1 && index>countV) {
		cout << "Введено не корректное значение";
		return 0;
	}
	delete[] listV;
	listV = nlist;
	countV--;
	return 0;
}

int EdgeList_Graph::DeleteEdge() {
	Edge* nlist = new Edge[countR - 1];
	int vert1, vert2;
	cout << "Введите вершины, ребро между которыми нужно удалить" << endl << "\tВершина из которой идёт ребро: ";
	cin >> vert1;
	cout << "\tВершина в которую идёт ребро: ";
	cin >> vert2;
	for (int i = 0; i < countR; i++) {
		if ((vert1 == listR[i].v1) && (vert2 == listR[i].v2)) {
			if (i == 0) {
				for (int j = 1; j < countR; j++) {
					nlist[i].v1 = listR[i].v1;
					nlist[i].v2 = listR[i].v2;
					nlist[i].weight = listR[i].weight;
				}
				delete[] listR;
				listR = nlist;
				countR--;
				return 0;
			}
			if (i == countR - 1) {
				for (int j = 0; j < countR - 1; j++) {
					nlist[i].v1 = listR[i].v1;
					nlist[i].v2 = listR[i].v2;
					nlist[i].weight = listR[i].weight;
				}
				delete[] listR;
				listR = nlist;
				countR--;
				return 0;
			}
			else {
				for (int j = 0; j < i; j++) {
					nlist[i].v1 = listR[i].v1;
					nlist[i].v2 = listR[i].v2;
					nlist[i].weight = listR[i].weight;
				}
				for (int j = i + 1; j < countR; j++) {
					nlist[i].v1 = listR[i].v1;
					nlist[i].v2 = listR[i].v2;
					nlist[i].weight = listR[i].weight;
				}
				delete[] listR;
				listR = nlist;
				countR--;
				return 0;
			}

		}
	}
	cout << endl << "\tДанного ребра не существует" << endl << endl;
	return 0;
}

int EdgeList_Graph::ChangeVertex() {
	int foo, foo2, foo3, foo4;
	string name;
	cout << "Введите индекс вершины, которую необходимо изменить: ";
	cin >> foo;
	cout << "Введите новый индес вершины: ";
	cin >> foo2;
	if (foo2 > countV) {
		listV[foo - 1].index = foo2;
		return 0;
	}
	else {
		cout << "\tЗаменить текущую вершину с индексом " << foo2 << "?" << endl << "\t[1] Да" << endl << "\t[2] Нет" << endl << "\t# ";
		cin >> foo3;
		if (foo3 == 1) {
			if (foo2 == 1) {
				foo4 = listV[0].index;
				name = listV[0].name;
				listV[0].index = listV[foo - 1].index;
				listV[0].name = listV[foo - 1].name;
				listV[foo - 1].index = foo4;
				listV[foo - 1].name = name;
				return 0;
			}
			else {
				for (int i = 0; i < countV; i++) {
					if (foo2 == listV[i].index) {
						foo4 = listV[i].index;
						name = listV[i].name;
						listV[i].index = listV[foo - 1].index;
						listV[i].name = listV[foo - 1].name;
						listV[foo - 1].index = foo4;
						listV[foo - 1].name = name;
						return 0;
					}
				}
			}
		}
		if (foo3 == 2) {
			ChangeVertex();
			return 0;
		}
		else {
			cout << "Неправильно введённое значение" << endl;
			return 0;
		}
	}
}

int EdgeList_Graph::ChangeEdge() {
	vector <int> can;
	int v = 0, w = 0, y = 0;
	cout << "\tВведите индекс вершины: ";
	cin >> v;
	for (int j = 0; j < countR; j++) {
		if (listR[j].v1 == v) {
			can.push_back(listR[j].v2);
			y++;
		}
	}
	if (y != 0) {
		cout << endl << "\tДоступные вершины" << endl;

		for (int j = 0; j < can.size(); j++) {
			cout << "\t[" << j + 1 << "]  " << can[j] << endl;
		}
		cout << endl << "\t# ";
		cin >> w;

		for (int j = 0; j < countR; j++) {
			if (((listR[j].v1 == v) && (listR[j].v2 == w)) || ((listR[j].v2 == v) && (listR[j].v1 == w))) {
				cout << endl << "\tVertex 1      Vertex2" << "      Weight" << endl;
				cout << "           " << listR[j].v1 << "             " << listR[j].v2 << "           " << listR[j].weight << endl << endl;
				cout << "\tВведите новое значение Vertex 1: ";
				cin >> v;
				listR[j].v1 = v;
				cout << "\tВведите новое значение Vertex 2: ";
				cin >> w;
				listR[j].v2 = w;
				for (int k = 0; k < countR; k++) {
					if ((listR[j].v1 == listR[k].v1) && (listR[j].v2 == listR[k].v2)) {
						cout << endl << "\tТакое ребро уже существует" << endl;
						return 0;
					}
				}
				cout << "\tВведите новое значение Weight: ";
				cin >> w;
				listR[j].weight = w;
				return 0;
			}
		}
	}
	else {
		cout << "\tСмежные вершины не найдены" << endl;
		return 0;
	}
}

int FIRST(int n, Vertex* vert) {
	for (int i = 0; i < n; i++) {
		if (vert[i].first == 1) return (vert[i].index);
	}
}

int NEXT(int n, int v, int i, Vertex* vert, Edge* edge) {
	for (int j = i; j < n; j++) {
		if (edge[j].v1 == v) return(edge[j].v2);
		if (edge[j].v2 == v) return(edge[j].v1);
	}
	cout << endl << "\tВершина не найдена" << endl;
	return (-1);
}

int EdgeList_Graph::VERTEX(int i, int v) {
	for (int j = 0; j < countR; j++) {
		if (listR[j].v1 == v) {
			if (listR[j].v2 == i) {
				cout << "\tВершина " << listR[j].v2 << " найдена";
				return 0;
			}
		}
		if (listR[j].v2 == v) {
			if (listR[j].v1 == i) {
				cout << "\tВершина " << listR[j].v1 << " найдена";
				return 0;
			}
		}
	}
	cout << "\tВершины, смежные с вершиной v не найдены" << endl << endl;
	return 0;
}


int main() {
	setlocale(LC_ALL, "Rus");
	int answer, first, next, v, i;
	TGF object;
	EdgeList_Graph graph;

	do {
		cout << "1: Считать файл" << endl;
		cout << "2: Показать список вершин" << endl;
		cout << "3: Показать список дуг" << endl;
		cout << "4: Изменить граф" << endl;
		cout << "5: Поиск внешнего радиуса (обход в ширину)" << endl;
		cout << "6: Функции FIRST, NEXT, VERTEX" << endl << endl;
		cout << "0: Завершить программу" << endl;
		cout << endl << "--------------------------------------------------------------------" << endl;
		cout << endl << "# ";
		cin >> answer;
		cout << endl;
		switch (answer) {
		case 1:
			do {
				object.OpenFile();
				if (object.flag == 0) {
					CountVR(&object, &graph);
				}
			} while (object.isopen == 0);
			graph.List();
			GetVR(&object, graph.getV(), graph.getR());
			break;
		case 2:
			graph.PrintV();
			break;
		case 3:
			graph.PrintR();
			break;
		case 4:
			graph.ChangeGraph();
			break;
		case 5:
			cout << "Внешний радиус: " << SearchRadius(&graph, graph.getR()) << endl;
			break;
		case 6:
			first = FIRST(graph.countV, graph.getV());
			cout << "\tИндекс первого элемента: " << first << endl << endl << endl;
			cout << "\tИндекс элемента, после которого следует найти вершину ";
			cin >> i;
			cout << "\tВершина должна быть смежна с вершиной под индексом ";
			cin >> v;
			next = NEXT(graph.countR, v, i, graph.getV(), graph.getR());
			cout << endl << "\tРезультат: " << next << endl << endl << endl;

			cout << "\tВведите индекс вершины i и вершины v" << endl << endl << "\tВершина i: ";
			cin >> i;
			cout << "\tВершина v: ";
			cin >> v;
			graph.VERTEX(i, v);
			break;
		case 0:
			return 1;
			break;
		default:
			cout << endl << "\tВыберите один из вариантов" << endl;
			break;
		}
	} while (answer != 0);

	return 0;
}
