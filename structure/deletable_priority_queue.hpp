#include "daylight/base.hpp"

template<typename T, typename vec = vector<T>,
		 typename cmp = less<typename vec::value_type>>
struct deletable_priority_queue {
private:
	map<T, T> M;
	priority_queue<T, vec, cmp> Q;
	int sz;

public:
	deletable_priority_queue() {
		sz = 0;
	}

	/// @brief キューに要素vを追加する
	/// @param v 追加する要素
	void push(T v) {
		Q.push(v);
		M[v]++;
		sz++;
	}
	/// @brief キューから要素vを削除する
	/// @param v 削除する要素
	/// @return 削除が成功したかどうか
	bool remove(T v) {
		if(M[v] == 0) return false;
		M[v]--;
		sz--;
		while(!Q.empty() && M[Q.top()] == 0) {
			Q.pop();
		}
		return true;
	}
	/// @brief キューの最大元を取り出す
	/// @return キューの最大元
	T top() {
		return Q.top();
	}
	/// @brief キューの最大元を削除する
	void pop() {
		while(M[Q.top()] == 0) {
			Q.pop();
		}
		M[Q.top()]--;
		sz--;
		Q.pop();
		while(!Q.empty() && M[Q.top()] == 0) {
			Q.pop();
		}
	}
	/// @brief キューが要素vを含むかを判定する
	/// @param v キューが含むかを判定する要素
	/// @return キューがvを含むかどうか
	bool contains(T v) {
		return M[v] > 0;
	}
	/// @brief キューの要素数を取得する
	/// @return キューの要素数
	size_t size() {
		return (size_t) sz;
	}
};