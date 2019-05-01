#include "pch.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <conio.h>

using namespace std;

int main()
{

	srand((unsigned)time(0));

	int x = 0, y = 0, chest = 0, row, col, MineNum;


	cout << "请输入场地行数：";

	cin >> row;

	cout << "请输入场地列数：";

	cin >> col;

	cout << "请输入地雷数：";

	cin >> MineNum;

	vector<vector<string> >ground(row, vector<string>(col, "■"));

	vector<vector<int> >mines(row, vector<int>(col, 0));

	vector<int>random_point(row * col, 0);

	for (;;) {

		if (MineNum > row * col * 0.3) {

			cout << "地雷数过多，请重新输入" << '\n';

			cin >> MineNum;

		}
		else {

			break;

		}

	}

	for (int i = 0; i <= MineNum; i++) {
	
		if (mines[rand() % row][rand() % col]) {
		
			i--;
		
		}
		else {
		
			mines[rand() % row][rand() % col] = 1;
		
		}
	
	}

	//正式流程
	for (;;) {

		system("cls");

		cout << "剩余地雷数：" << MineNum << '\n';

		for (int i = 0; i < row; i++) {

			for (int j = 0; j < col; j++) {

				if (i == y && j == x) {

					cout << "⊙";

				}
				else {

					cout.width(2);
					cout << ground[i][j];
				}

			}

			cout << '\n';

		}
		cout << '\n';

		if (chest) {

			for (int i = 0; i < row; i++) {

				for (int j = 0; j < col; j++) {

					cout << mines[i][j];

				}

				cout << '\n';

			}
		}

		cout << '\n' << "当前位置：" << ground[y][x] << endl;

		switch (_getch())
		{
		case 100:
			if (x < col - 1) {
				x++;
			}
			break;

		case 119:
			if (y > 0) {
				y--;
			}
			break;

		case 97:
			if (x > 0) {
				x--;
			}
			break;

		case 115:
			if (y < row - 1) {
				y++;
			}
			break;

		case 13:
			//翻开
			if (mines[y][x] && (ground[y][x] == "■")) {
			
				goto fail;
			
			}
			else if (ground[y][x] == "■"){
			
				ground[y][x] = "?";

			begin :
				for (int m = 0; m < row; m++) {

					for (int n = 0; n < col; n++) {

						if (ground[m][n] == "?") {

							goto part1;//翻格子，临时改为问号的部分

						}
						else {
							
							continue;
							
						}

					part1 : 
						int mine = 0;
							
						for (int i = m - 1; i < m + 2; i++) {
				
							if (i < 0 || i >= row) {
					
								continue;
					
							}
							else {
					
								for (int j = n - 1; j < n + 2; j++) {
						
									if (j < 0 || j >= col) {
							
										continue;
							
									}
									else {
							
										if (mines[i][j]) {
												
											mine++;
												
										}
							
									}
						
								}
					
							}
				
						}

						if (mine) {
								
							ground[m][n] = '0' + mine;
								
						}
						else {

							ground[m][n] = "□";
								
							for (int i = m - 1; i < m + 2; i++) {

								if (i < 0 || i >= row) {

									continue;

								}
								else {

									for (int j = n - 1; j < n + 2; j++) {

										if (j < 0 || j >= col) {

											continue;

										}
										else {
											if (ground[i][j] == "■") {
													
												ground[i][j] = '?';

											}
													
										}

									}

								}

							}

						}
								
						goto begin;
							
					}

				}

			}
		
			for (int i = 0; i < row; i++) {

				for (int j = 0; j < col; j++) {

					if (ground[i][j] == "■" && !mines[i][j]) {

						goto out;

					}

				}

			}

			goto win;

		out:
			break;

		case 32 :
			if (ground[y][x] == "■") {

				ground[y][x] = "※";

				if (mines[y][x] == 1) {

					MineNum--;

				}

			}
			else if (ground[y][x] == "※") {

				ground[y][x] = "■";
	
				if (mines[y][x] == 1) {

					MineNum++;

				}

			}
			break;

		}
	}

win:
	system("cls");

	cout << "CONGRATULATIONS!!!!\n" << endl;

	for (int i = 0; i < row; i++) {

		for (int j = 0; j < col; j++) {

			if (mines[i][j]) {

				cout << "※";
	
			}
			else {

				cout.width(2);
				cout << ground[i][j];
			}

		}

		cout << endl;

	}

	goto end;
	
fail:
	system("cls");

	cout << "GAME OVER!!!\n" << endl;

	for (int i = 0; i < row; i++) {

		for (int j = 0; j < col; j++) {

			if (mines[i][j]) {

				cout.width(2);
				cout <<'X';

			}
			else {

				cout.width(2);
				cout << ground[i][j];
			}

		}

		cout << endl;

	}

	goto end;

end:

	return 0;
}
