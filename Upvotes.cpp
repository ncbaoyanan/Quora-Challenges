#include "iostream"
#include "vector"

using namespace std;

long comb(long x) {
	return x*(x-1)/2;
}

void solveVotes(vector<long>& nums, long N, long K) {
	vector<pair<long, long>> incRanges;
	vector<pair<long, long>> decRanges;

	// contiguous ranges
	long incBegin = 0;
	long decBegin = 0;
	long i = 1;
	for(; i < nums.size(); i++) {
		if (nums[i] < nums[i-1]) {
			if (i > incBegin + 1) {
				incRanges.push_back(make_pair(incBegin, i-1));
			}
			incBegin = i;
		}
		if (nums[i] > nums[i-1]) {
			if (i > decBegin + 1) {
				decRanges.push_back(make_pair(decBegin, i-1));
			}
			decBegin = i;
		}
	}
	if (i > incBegin + 1) {
		incRanges.push_back(make_pair(incBegin, i-1));
	}
	if (i > decBegin + 1) {
		decRanges.push_back(make_pair(decBegin, i-1));
	}

	// calculate the combinations offline
	vector<long> incComb(incRanges.size(), 0);
	for(long i = 0; i < incRanges.size(); i++) {
		incComb[i] = comb(incRanges[i].second - incRanges[i].first + 1);
	}
	vector<long> decComb(decRanges.size(), 0);
	for(long i = 0; i < decRanges.size(); i++) {
		decComb[i] = comb(decRanges[i].second - decRanges[i].first + 1);
	}

	long incStart = 0;
	long incEnd = 0;
	long decStart = 0;
	long decEnd = 0;

	for(long front = 0; front < N - K + 1; front++) {
		auto back = front + K - 1;
		long sum = 0;

		// non-decreasing
		if (!incRanges.empty()) {
			while(incStart < incRanges.size() && front > incRanges[incStart].second) {
				incStart++;
			}
			while(incEnd < incRanges.size() && back > incRanges[incEnd].second) {
				incEnd++;
			}
			if (incStart < incEnd) {
				if (front > incRanges[incStart].first) {
					if(front < incRanges[incStart].second) {
						sum += comb(incRanges[incStart].second - front + 1);
					}
				} else {
					sum += incComb[incStart];
				}
				for(long i = incStart + 1; i < incEnd; i++){
					sum += incComb[i];
				}
			}
			if (incEnd < incRanges.size() && back > incRanges[incEnd].first) {
				sum += comb(min(back - incRanges[incEnd].first + 1, K));
			}
		}
		// non-increasing
		if (!decRanges.empty()) {
			while(decStart < decRanges.size() && front > decRanges[decStart].second) {
				decStart++;
			}
			while(decEnd < decRanges.size() && back > decRanges[decEnd].second) {
				decEnd++;
			}
			if (decStart < decEnd) {
				if (front > decRanges[decStart].first) {
					if(front < decRanges[decStart].second) {
						sum -= comb(decRanges[decStart].second - front + 1);
					}
				} else {
					sum -=  decComb[decStart];
				}
				for(long i = decStart + 1; i < decEnd; i++){
					sum -= decComb[i];
				}
			}
			if (decEnd < decRanges.size() && back > decRanges[decEnd].first) {
				sum -= comb(min(back - decRanges[decEnd].first + 1, K));
			}
		}
		cout << sum << endl;
	}
}

int main()
{
	long N;
	long K;
	cin >> N >> K;
	vector<long> nums(N, 0);
	for(long i = 0; i < N; i++) {
		cin >> nums[i];
	}
	solveVotes(nums, N, K);
	return 0;
}
