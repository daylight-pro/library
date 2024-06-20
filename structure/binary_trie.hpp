#include "daylight/base.hpp"
template<typename T, int MAX_LOG = 32>
struct BinaryTrie {
	struct Node {
		vi next;  //次の頂点番号
		int common;	 //この頂点を通る文字列の数
		T lazy;
		Node() {
			next.assign(2, -1);
			common = 0;
			lazy = 0;
		}
	};

	vector<Node> nodes;
	/// @brief 空のトライ木を生成
	BinaryTrie() {
		nodes.push_back(Node());
	}

	/// @brief 全要素についてvalとのXORで上書きする
	/// @param val
	void apply_xor(T val) {
		nodes[0].lazy ^= val;
	}

private:
	/// @brief 遅延伝播
	/// @param cur ノード番号
	/// @param b ビット番号
	void push(int cur, int b) {
		if((nodes[cur].lazy >> (T) b) & (T) 1) {
			swap(nodes[cur].next[0], nodes[cur].next[1]);
		}
		REP(i, 2) {
			if(nodes[cur].next[i] != -1) {
				nodes[nodes[cur].next[i]].lazy
					^= nodes[cur].lazy;
			}
		}
		nodes[cur].lazy = 0;
	}

public:
	/// @brief valをトライ木に追加する
	/// @param val 追加する値
	/// @param cur (ノード番号)
	/// @param b (ビット番号)
	void add(T val, int cur = 0, int b = MAX_LOG - 1) {
		nodes[cur].common++;
		if(b < 0) {
			return;
		}
		push(cur, b);
		int nxt = (val >> (T) b) & (T) 1;
		if(nodes[cur].next[nxt] == -1) {
			nodes[cur].next[nxt] = SZ(nodes);
			nodes.push_back(Node());
		}
		add(val, nodes[cur].next[nxt], b - 1);
	}
	/// @brief valをトライ木から削除する
	/// @param val 削除する値
	/// @param cur (ノード番号)
	/// @param b (ビット番号)
	void erase(T val, int cur = 0, int b = MAX_LOG - 1) {
		assert(cur != -1);
		nodes[cur].common--;
		if(b < 0) {
			return;
		}
		push(cur, b);
		int nxt = (val >> (T) b) & (T) 1;
		erase(val, nodes[cur].next[nxt], b - 1);
	}
	/// @brief 全要素をbxorとのXORで置き換えたとき,最小の値
	/// @param bxor (任意)XORを取る値
	/// @param cur (ノード番号)
	/// @param b (ビット番号)
	/// @return 最小値
	T min_element(T bxor = 0, int cur = 0,
				  int b = MAX_LOG - 1) {
		if(b < 0) return 0;
		push(cur, b);
		int nxt = (bxor >> (T) b) & (T) 1;
		int ind = nodes[cur].next[nxt];
		if(ind == -1 || nodes[ind].common == 0) {
			nxt = nxt ^ 1;
		}
		return min_element(bxor, nodes[cur].next[nxt],
						   b - 1)
			| ((T) nxt << (T) b);
	}
	/// @brief 全要素をbxorとのXORで置き換えたとき,最大値を求める
	/// @param bxor (任意)XORを取る値
	/// @param cur (ノード番号)
	/// @param b (ビット番号)
	/// @return 最大値
	T max_element(T bxor = 0) {
		return min_element(~bxor);
	}
	/// @brief val未満の要素の数を求める
	/// @param val 検索する値
	/// @param cur (ノード番号)
	/// @param b (ビット番号)
	int lower_bound_rank(T val, int cur = 0,
						 int b = MAX_LOG - 1) {
		if(cur == -1 || b < 0) return 0;
		push(cur, b);
		int nxt = (val >> (T) b) & (T) 1;
		int ret = lower_bound_rank(
			val, nodes[cur].next[nxt], b - 1);
		if(nxt == 1 && nodes[cur].next[0] != -1) {
			ret += nodes[nodes[cur].next[0]].common;
		}
		return ret;
	}

	/// @brief val以下の要素の数を求める
	/// @param val 検索する値
	/// @param cur (ノード番号)
	/// @param b (ビット番号)
	int upper_bound_rank(T val) {
		return lower_bound_rank(val + 1);
	}

	int range_freq(T l, T r) {
		return lower_bound_rank(r) - lower_bound_rank(l);
	}

	/// @brief k番目に小さい要素を求める
	/// @param k 求める要素の順位(0-indexed)
	/// @param cur (ノード番号)
	/// @param b (ビット番号)
	/// @return k番目に小さい要素
	T kth_smallest(int k, int cur = 0,
				   int b = MAX_LOG - 1) {
		if(b < 0) return 0;
		int lower_ind = nodes[cur].next[0];
		int lower_cnt = 0;
		if(lower_ind != -1) {
			lower_cnt = nodes[lower_ind].common;
		}
		if(k < lower_cnt) {
			return kth_smallest(k, nodes[cur].next[0],
								b - 1);
		} else {
			return kth_smallest(k - lower_cnt,
								nodes[cur].next[1], b - 1)
				| ((T) 1 << (T) b);
		}
	}
	/// @brief k番目に大きい要素を求める
	/// @param k 求める要素の順位(0-indexed)
	/// @return k番目に大きい要素
	T kth_largest(int k) {
		return kth_smallest(SZ(size()) - 1 - k);
	}

	T operator[](int k) {
		return kth_smallest(k);
	}

	/// @brief valがトライ木にいくつ格納されているか求める
	/// @param val 検索する値
	int count(T val) {
		int cur = 0;
		REPR(b, MAX_LOG) {
			push(cur, b);
			cur = nodes[cur].next[(val >> (T) b) & (T) 1];
			if(cur == -1) return 0;
		}
		return nodes[cur].common;
	}
	/// @brief トライ木に格納された整数の数を数える
	/// @return トライ木に格納された整数の数
	size_t size() const {
		return nodes[0].common;
	}
	/// @brief トライ木が空か判定する
	/// @return トライ木が空ならtrue
	bool empty() const {
		return nodes[0].common == 0;
	}
};