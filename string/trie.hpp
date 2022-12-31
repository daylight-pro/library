#include "daylight/base.hpp"
template<int alphabet, char base>
struct Trie {
	struct Node {
		vi next;  //次の頂点番号
		int num;  //この頂点で終わる文字列の数
		int common;	 //この頂点を通る文字列の数
		Node() {
			next.assign(alphabet, -1);
			num = 0;
			common = 0;
		}
	};
	vector<Node> nodes;
	Trie() {
		nodes.push_back(Node());
	}
	/// @brief 単語wordを挿入
	/// @param word 挿入する単語
	void insert(const string& word) {
		int cur = 0;
		nodes[0].common++;
		REP(i, SZ(word)) {
			if(nodes[cur].next[word[i] - base] == -1) {
				nodes[cur].next[word[i] - base]
					= node_count();
				cur = node_count();
				nodes.push_back(Node());
				nodes[cur].common++;
			} else {
				cur = nodes[cur].next[word[i] - base];
				nodes[cur].common++;
			}
		}
		nodes[cur].num++;
	}
	/// @brief 単語を検索する。prefixがtrueなら単語をprefixに持つ単語があるか調べる
	/// @param word 検索する単語
	/// @param prefix 検索をprefixで行うかどうか
	/// @return 見つかったらtrue
	bool search(const string& word, bool prefix = false) {
		int cur = 0;
		REP(i, SZ(word)) {
			cur = nodes[cur].next[word[i] - base];
			if(cur == -1) {
				return false;
			}
		}
		return prefix || nodes[cur].num > 0;
	}
	/// @brief wordに沿って、Trie木をdfsする。
	/// @param word 進行方向を表す単語
	/// @param f f(node,i)i番目の文字までに対応するノードがnodeだった時の処理
	void dfs(const string& word,
			 const function<void(Node&, int)>& f) {
		int cur = 0;
		f(nodes[cur], 0);
		REP(i, SZ(word)) {
			cur = nodes[cur].next[word[i] - base];
			if(cur == -1) return;
			f(nodes[cur], i + 1);
		}
	}
	/// @brief Trie木に挿入された単語の数を取得
	/// @return 挿入された単語数
	size_t size() const {
		return nodes[0].common;
	}
	/// @brief 現在のTrie木のノード数を求める
	/// @return 現在のノード数
	int node_count() const {
		return SZ(nodes);
	}
};