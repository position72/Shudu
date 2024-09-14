#include <iostream>
#include <cstdio>
#include <assert.h>
#include <set>
#include <vector>
using namespace std;

bool col_num_exists[10][10];
bool row_num_exists[10][10];
bool ninegrid_num_exists[9][10];
int ninegrid_num_count[9];
inline int coord_to_ninegrid_id(int i, int j)
{
	return 3 * (i / 3) + (j / 3);
}
//int chessboard[9][9] =
//{
//	{3, 1, 5, 0, 9, 7, 4, 0, 0},
//	{4, 2, 0, 3, 0, 0, 0, 7, 0},
//	{7, 0, 8, 0, 6, 0, 3, 0, 0},
//	{0, 6, 4, 0, 2, 9, 0, 0, 0},
//	{8, 3, 9, 0, 7, 4, 0, 0, 2},
//	{0, 7, 2, 1, 0, 0, 0, 9, 0},
//	{0, 0, 0, 9, 1, 0, 5, 8, 0},
//	{0, 5, 0, 0, 8, 3, 9, 0, 0},
//	{0, 8, 1, 0, 4, 5, 2, 3, 6},
//};

int chessboard[9][9] =
{
	{0, 4, 6, 9, 0, 3, 0, 0, 0},
	{0, 0, 3, 0, 5, 0, 0, 6, 0},
	{9, 0, 0, 0, 0, 2, 0, 0, 3},
	{0, 0, 5, 0, 0, 6, 0, 0, 0},
	{8, 0, 0, 0, 0, 0, 0, 1, 0},
	{0, 1, 0, 7, 8, 0, 2, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 5, 0},
	{0, 8, 1, 3, 0, 0, 0, 0, 7},
	{0, 0, 0, 8, 0, 0, 1, 0, 4},
};


int zero_count = 0;
void set_num_in_chessboard(int r, int c, int num)
{
	col_num_exists[c][num] = true;
	row_num_exists[r][num] = true;
	ninegrid_num_exists[coord_to_ninegrid_id(r, c)][num] = true;
	ninegrid_num_count[coord_to_ninegrid_id(r, c)]++;

	assert(ninegrid_num_count[coord_to_ninegrid_id(r, c)] <= 9);
	chessboard[r][c] = num;
	--zero_count;
}

int main()
{
	memset(col_num_exists, 0, sizeof(col_num_exists));
	memset(row_num_exists, 0, sizeof(row_num_exists));
	memset(ninegrid_num_count, 0, sizeof(ninegrid_num_count));
	
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			int num = chessboard[i][j];
			assert(num >= 0 && num <= 9);

			zero_count += num == 0;

			col_num_exists[j][num] = true;
			row_num_exists[i][num] = true;
			ninegrid_num_exists[coord_to_ninegrid_id(i, j)][num] = true;
			if (num) ninegrid_num_count[coord_to_ninegrid_id(i, j)]++;
		}
	}
	
	while (zero_count)
	{
		for (int num = 1; num <= 9; ++num)
		{
			if (num == 6)
			{
				std::cout << "asd";
			}
			for (int c = 0; c < 9; ++c)
			{
				if (col_num_exists[c][num]) continue;

				int base_r = 0, pos_r = -1;
				bool success = true;
				
				for (int base_r = 0; base_r < 9; base_r += 3)
				{					
					// 考虑一个九宫格内的情况
					bool skip_c = false;
					int ninegrid_id = coord_to_ninegrid_id(base_r, c);
					if (ninegrid_num_exists[ninegrid_id][num]) continue;

					int pos_r_in_ninegrid = -1;
					int zero_count_in_ninegrid = 9 - ninegrid_num_count[ninegrid_id];
					
					// 用九宫格外的num，则排除九宫格内的空位
					set<int> ers;
					for (int rr = 0; rr < 9; ++rr)
					{
						if (row_num_exists[rr][num])
						{
							for (int cc = 0; cc < 9; ++cc)
							{
								if (ninegrid_id == coord_to_ninegrid_id(rr, cc) && chessboard[rr][cc] == 0 && ers.count(rr * 9 + cc) == 0)
								{
									zero_count_in_ninegrid--;
									ers.insert(rr * 9 + cc);
								}
							}
						}
					}
					for (int cc = 0; cc < 9; ++cc)
					{
						if (col_num_exists[cc][num])
						{
							for (int rr = 0; rr < 9; ++rr)
							{
								if (ninegrid_id == coord_to_ninegrid_id(rr, cc) && chessboard[rr][cc] == 0 && ers.count(rr * 9 + cc) == 0)
								{
									zero_count_in_ninegrid--;
									ers.insert(rr * 9 + cc);
								}
							}
						}
					}
					for (int offset_r = 0; offset_r < 3; ++offset_r)
					{
						int r = base_r + offset_r;						

						if (row_num_exists[r][num] || chessboard[r][c] != 0) continue;

						// 一个九宫格内的一列有两个位置可放置，该列则不能确定
						if (pos_r_in_ninegrid != -1)
						{
							skip_c = true;
							success = false;
							break;
						}
						pos_r_in_ninegrid = r;						
					}

					if (skip_c) break;

					// 一个九宫格内只有一个位置可放置，则该列的num位置可确定
					if (zero_count_in_ninegrid == 1 && pos_r_in_ninegrid != -1)
					{						
						pos_r = pos_r_in_ninegrid;
						break;
					}
					
					// 该列有两个位置可放置，该列的num位置不能确定
					if (pos_r_in_ninegrid != -1)
					{
						if (pos_r != -1)
						{
							success = false;
							break;
						}

						pos_r = pos_r_in_ninegrid;
					}
				}
				
				if (pos_r == -1) success = false;

				if (success)
				{
					set_num_in_chessboard(pos_r, c, num);
				}
			}

			for (int r = 0; r < 9; ++r)
			{
				if (row_num_exists[r][num]) continue;

				int base_c = 0, pos_c = -1;
				bool success = true;

				for (int base_c = 0; base_c < 9; base_c += 3)
				{
					// 考虑一个九宫格内的情况
					bool skip_r = false;
					int ninegrid_id = coord_to_ninegrid_id(r, base_c);
					if (ninegrid_num_exists[ninegrid_id][num]) continue;

					int pos_c_in_ninegrid = -1;
					int zero_count_in_ninegrid = 9 - ninegrid_num_count[ninegrid_id];

					// 用九宫格外的num，则排除九宫格内的空位
					set<int> ers;
					for (int rr = 0; rr < 9; ++rr)
					{
						if (row_num_exists[rr][num])
						{
							for (int cc = 0; cc < 9; ++cc)
							{
								if (ninegrid_id == coord_to_ninegrid_id(rr, cc) && chessboard[rr][cc] == 0 && ers.count(rr * 9 + cc) == 0)
								{
									zero_count_in_ninegrid--;
									ers.insert(rr * 9 + cc);
								}
							}
						}
					}
					for (int cc = 0; cc < 9; ++cc)
					{
						if (col_num_exists[cc][num])
						{
							for (int rr = 0; rr < 9; ++rr)
							{
								if (ninegrid_id == coord_to_ninegrid_id(rr, cc) && chessboard[rr][cc] == 0 && ers.count(rr * 9 + cc) == 0)
								{
									zero_count_in_ninegrid--;
									ers.insert(rr * 9 + cc);
								}
							}
						}
					}
					for (int offset_c = 0; offset_c < 3; ++offset_c)
					{
						int c = base_c + offset_c;

						if (col_num_exists[c][num] || chessboard[r][c] != 0) continue;

						// 一个九宫格内的一列有两个位置可放置，该列则不能确定
						if (pos_c_in_ninegrid != -1)
						{
							skip_r = true;
							success = false;
							break;
						}
						pos_c_in_ninegrid = c;
					}

					if (skip_r) break;

					// 一个九宫格内只有一个位置可放置，则该列的num位置可确定
					if (zero_count_in_ninegrid == 1 && pos_c_in_ninegrid != -1)
					{
						pos_c = pos_c_in_ninegrid;
						break;
					}

					// 该列有两个位置可放置，该列的num位置不能确定
					if (pos_c_in_ninegrid != -1)
					{
						if (pos_c != -1)
						{
							success = false;
							break;
						}

						pos_c = pos_c_in_ninegrid;
					}
				}

				if (pos_c == -1) success = false;

				if (success)
				{
					set_num_in_chessboard(r, pos_c, num);
				}
			}

			for (int id = 0; id < 9; ++id)
			{
				if (ninegrid_num_exists[id][num]) continue;

				if (ninegrid_num_count[id] == 9) continue;

				std::vector<int> lost_nums;
				for (int lost_number = 1; lost_number <= 9; ++lost_number)
				{
					if (!ninegrid_num_exists[id][lost_number])
					{
						lost_nums.push_back(lost_number);
					}
				}
				assert(lost_nums.size() > 0);

				int base_r = id / 3 * 3, base_c = id % 3 * 3;
				bool success = false;
				for (int offset_r = 0; offset_r < 3; ++offset_r)
				{
					if (success) break;
					for (int offset_c = 0; offset_c < 3; ++offset_c)
					{
						int r = base_r + offset_r;
						int c = base_c + offset_c;
						if (chessboard[r][c]) continue;

						int num_of_cannot_here = 0;
						for (int lost_num : lost_nums)
						{
							if (row_num_exists[r][lost_num] || col_num_exists[c][lost_num])
							{
								num_of_cannot_here++;
							}
						}

						if (num_of_cannot_here == lost_nums.size() - 1 && !row_num_exists[r][num] && !col_num_exists[c][num])
						{
							success = true;
							set_num_in_chessboard(r, c, num);
							break;
						}
					}
				}
			}
		}

		cout << "----------------------------------------------------------------------------\n";
		for (int i = 0; i < 9; ++i)
		{
			for (int j = 0; j < 9; ++j)
			{
				cout << chessboard[i][j] << " ";
			}
			cout << endl;
		}
	}

	cout << "----------------------------------------------------------------------------\n";
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			cout << chessboard[i][j] << " ";
		}
		cout << endl;
	}
}