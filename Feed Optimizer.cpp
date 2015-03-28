// test1.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
using namespace std;
#include "iostream"
#include "fstream"
#include <vector>  
#include <list>

class Story{
public:
	int id;
	long time;
	long score;
	long height;
	Story(int i, long t, long s, long h){
		id = i;
		time = t;
		score = s;
		height = h;
	}
};
class Stories{
public:
	vector<Story> data;
	int N;
	int W;
	int H;
	int ibegin;
	int ID;
	int iend;
	int ilastbegin;
	int ilastend;
	int imid;
	list<int> lastsol;
	long lastscore;
	int lastcount;
	int lastnitems;
	vector<vector<long>> frontopt;
	vector<vector<int>> frontpath;
	vector<vector<long>> rearopt;
	vector<vector<int>> rearpath;
	Stories(int n, int w, int h){
		N = n;
		W = w;
		H = h;
		ID = 0;
		ibegin = 0;
		iend = 0;
		ilastbegin = -1;
		ilastend = -1;
		imid = - 1;
		frontopt = vector<vector<long>>(1, vector<long>(H+1,0));
		frontpath = vector<vector<int>>(1, vector<int>(H+1,-2));
		rearopt = vector<vector<long>>(1, vector<long>(H+1,0));
		rearpath = vector<vector<int>>(1, vector<int>(H+1,-2));
	}
	void insert(long t, long s, long h){
		ID ++;
		if (h <= H){
			Story one(ID, t, s, h);
			data.push_back(one);
			iend++;
		}
	}
	void check(long t){
		while(ibegin < data.size()){
			if(data[ibegin].time < t - W){
				ibegin ++;
			}
			else
				break;
		}

		//Case 1:
		if((ibegin == ilastbegin) && (iend == ilastend)){
			cout<<lastscore <<" "<<lastcount;
			for (list<int>::iterator it=lastsol.begin(); it != lastsol.end(); ++it)
				cout << ' ' << *it;
			cout<<endl;
			return;
		}
		
		//Case 2:
		if(ibegin < imid){
			while(ilastend < iend){
				ilastend++;
				rearopt.push_back(vector<long>(H+1,0));
				rearpath.push_back(vector<int>(H+1,-2));
				for(int ih = 1; ih <= H; ih++){
					if(data[ilastend-1].height <= ih){
						long without = rearopt[rearopt.size()-2][ih];
						long with = rearopt[rearopt.size()-2][ih - data[ilastend-1].height] + data[ilastend-1].score;
						rearopt[rearopt.size()-1][ih] = max(without, with);
						if(without > with){
							rearpath[rearopt.size()-1][ih] = -1;
						}
						else if(without < with){
							rearpath[rearopt.size()-1][ih] = 1;
						}
						else{
							rearpath[rearopt.size()-1][ih] = 0;
						}
					}
					else{
						rearopt[rearopt.size()-1][ih] = rearopt[rearopt.size()-2][ih];
					}					
				}
			}
			long maxscore = 0;
			int ifrontin = lastnitems - imid + ibegin;
			int irearin = rearopt.size()-1;
			for(int ihf = 0; ihf <= H; ihf++){
				maxscore = max(frontopt[ifrontin][ihf] + rearopt[irearin][H-ihf], maxscore);
			}
			vector<list<int>> solgvec;
			if(maxscore > 0){
				for(int ihf = 0; ihf <= H; ihf++){
					if (frontopt[ifrontin][ihf] + rearopt[irearin][H-ihf] == maxscore){
						list<int> solf;
						list<int> solr;

						//find the solf
						if(ihf > 0) {
							vector<list<int>> solvec;
							solvec.push_back(list<int>());
							vector<int> hvec;
							hvec.push_back(ihf);
							int h;
							for(int i = ifrontin; i < lastnitems; i++){
								int hsize =  hvec.size();
								int numzeros = 0;
								for(int ih = 0; ih < hsize; ih++){
									h = hvec[ih];
									if(h == 0){
										numzeros++;
										continue;
									}
									int fastvar = imid - lastnitems + i;
									if(frontpath[i][h] == 1){
										solvec[ih].push_back(data[fastvar].id);
										hvec[ih] -= data[fastvar].height;
									}
									else if(frontpath[i][h] == 0){
										hvec.push_back(hvec[ih] - data[fastvar].height);
										solvec.push_back(solvec[ih]);
										solvec[hvec.size()-1].push_back(data[fastvar].id);
									}
								
								}
								if(numzeros == hsize)
									break;
							}
							int nsol = solvec.size();
							//Pick up the best subset
							int shortest = 32767;
							int best;
							for(int i = 0; i < nsol; i++){
								if(solvec[i].size() < shortest){
									shortest = solvec[i].size();
									best = i;
								}
								else if(solvec[i].size() == shortest){
									list<int>::iterator it1=solvec[i].begin();
									list<int>::iterator it2=solvec[best].begin();
									while(it1 != solvec[i].end()){
										if(*it1 < *it2){
											best = i;
											break;
										}
										else if(*it1 > *it2){
											break;
										}
										++it1; 
										++it2;
									}
								}
							}
							solf = solvec[best];
						}

						//find the solr
						if(H - ihf > 0){
							vector<list<int>> solvec;
							solvec.push_back(list<int>());
							vector<int> hvec;
							hvec.push_back(H - ihf);
							int h;
							for(int i = irearin; i > 0; i--){
								int hsize =  hvec.size();
								int numzeros = 0;
								for(int ih = 0; ih < hsize; ih++){
									h = hvec[ih];
									if(h == 0){
										numzeros++;
										continue;
									}
									int fastvar = imid + i - 1;
									if(rearpath[i][h] == 1){
										solvec[ih].push_front(data[fastvar].id);
										hvec[ih] -= data[fastvar].height;
									}
									else if(rearpath[i][h] == 0){
										hvec.push_back(hvec[ih] - data[fastvar].height);
										solvec.push_back(solvec[ih]);
										solvec[hvec.size()-1].push_front(data[fastvar].id);
									}								
								}
								if(numzeros == hsize)
									break;
							}
							int nsol = solvec.size();
							//Pick up the best subset
							int shortest = 32767;
							int best;
							for(int i = 0; i < nsol; i++){
								if(solvec[i].size() < shortest){
									shortest = solvec[i].size();
									best = i;
								}
								else if(solvec[i].size() == shortest){
									list<int>::iterator it1=solvec[i].begin();
									list<int>::iterator it2=solvec[best].begin();
									while(it1 != solvec[i].end()){
										if(*it1 < *it2){
											best = i;
											break;
										}
										else if(*it1 > *it2){
											break;
										}
										++it1; 
										++it2;
									}
								}
							}
							solr = solvec[best];
						}
						//combine them
						solf.insert(solf.end(), solr.begin(), solr.end());

						if(solgvec.size() > 0){
							list<int>::iterator it1 = solgvec[solgvec.size() - 1].begin();
							list<int>::iterator it2 = solf.begin();
							while((it1 != solgvec[solgvec.size() - 1].end()) && (it2 != solf.end()) && (*it1 == *it2)){
								it1++;
								it2++;
							}
							if(!((it1 == solgvec[solgvec.size() - 1].end()) && (it2 == solf.end())))
								solgvec.push_back(solf);
						}
						else{
							solgvec.push_back(solf);
						}
					}
				}
			}
			int nsol = solgvec.size();

			//Pick up the best subset
			int shortest = 32767;
			int best;
			for(int i = 0; i < nsol; i++){
				if(solgvec[i].size() < shortest){
					shortest = solgvec[i].size();
					best = i;
				}
				else if(solgvec[i].size() == shortest){
					list<int>::iterator it1=solgvec[i].begin();
					list<int>::iterator it2=solgvec[best].begin();
					while(it1 != solgvec[i].end()){
						if(*it1 < *it2){
							best = i;
							break;
						}
						else if(*it1 > *it2){
							break;
						}
						++it1; 
						++it2;
					}
				}
			}
			list<int> sol = solgvec[best];
			cout<<maxscore;
			cout<<" " << shortest;
			for (list<int>::iterator it=sol.begin(); it != sol.end(); ++it)
				cout << ' ' << *it;
			cout<<endl;
			lastscore = maxscore;
			lastcount = shortest;
			ilastbegin = ibegin;
			lastsol = sol;
			return;
		}
		



		//Case 3:
		int Nitems  = data.size() - ibegin;
		frontopt = vector<vector<long>>(Nitems+1, vector<long>(H+1,0));
		frontpath = vector<vector<int>>(Nitems+1, vector<int>(H+1,-2));
		for(int i = Nitems - 1; i >= 0; i--){
			for(int j = 1; j <= H; j++){
				if(data[ibegin + i].height <= j){
					long without = frontopt[i+1][j];
					long with = frontopt[i+1][j - data[ibegin + i].height] + data[ibegin + i].score;
					frontopt[i][j] = max(without, with);
					if(without > with){
						frontpath[i][j] = -1;
					}
					else if(without < with){
						frontpath[i][j] = 1;
					}
					else{
						frontpath[i][j] = 0;
					}
				}
				else{
					frontopt[i][j] = frontopt[i+1][j];
				}
			}
		}
		//Find all the subsets
		list<int> sol;
		vector<list<int>> solvec;
		solvec.push_back(sol);
		vector<int> hvec;
		hvec.push_back(H);
		int h;
		for(int i = 0; i < Nitems; i++){
			int hsize =  hvec.size();
			int numzeros = 0;
			for(int ih = 0; ih < hsize; ih++){
				h = hvec[ih];
				if(h == 0){
					numzeros++;
					continue;
				}
				if(frontpath[i][h] == 1){
					solvec[ih].push_back(data[ibegin + i].id);
					hvec[ih] -= data[ibegin + i].height;
				}
				else if(frontpath[i][h] == 0){
					hvec.push_back(hvec[ih] - data[ibegin + i].height);
					solvec.push_back(solvec[ih]);
					solvec[hvec.size()-1].push_back(data[ibegin + i].id);
				}				
			}
			if(numzeros == hsize)
				break;
		}
		int nsol = solvec.size();
		//Pick up the best subset
		int shortest = 32767;
		int best;
		for(int i = 0; i < nsol; i++){
			if(solvec[i].size() < shortest){
				shortest = solvec[i].size();
				best = i;
			}
			else if(solvec[i].size() == shortest){
				list<int>::iterator it1=solvec[i].begin();
				list<int>::iterator it2=solvec[best].begin();
				while(it1 != solvec[i].end()){
					if(*it1 < *it2){
						best = i;
						break;
					}
					else if(*it1 > *it2){
						break;
					}
					++it1; 
					++it2;
				}
			}
		}
		sol = solvec[best];
		cout<<frontopt[0][H];
		cout<<" " << shortest;
		for (list<int>::iterator it=sol.begin(); it != sol.end(); ++it)
			cout << ' ' << *it;
		cout<<endl;
		ilastbegin = ibegin;
		ilastend = iend;
		lastsol = sol;
		lastcount = shortest;
		lastscore = frontopt[0][H];
		lastnitems = Nitems;
		imid = iend;
		rearopt = vector<vector<long>>(1, vector<long>(H+1,0));
		rearpath = vector<vector<int>>(1, vector<int>(H+1,-2));
	}
};

int main( int argc, const char* argv[] ){
	int N;
	int W;
	int H;
	cin >> N >> W >> H;
	char type;
	Stories x(N, W, H);
	while(cin >> type){
		if(type == 'S'){
			long time;
			long score;
			long height;
			cin >> time>> score>> height;
			x.insert(time, score, height);
		}
		else if(type == 'R'){
			long chtime;
			cin >> chtime;
			x.check(chtime);
		}
	}
	return 0;
}
