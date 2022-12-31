
#include "daylight/base.hpp"

template<typename T>
struct priority_deque {
private:
	multiset<T> S;

public:
	/// @brief キューに要素vを追加する
	/// @param v 追加する要素
	void push(T v) {
		S.insert(v);
	}
	/// @brief キューから要素vを削除する
	/// @param v 削除する要素
	/// @return 削除が成功したかどうか
	bool del(T v) {
		auto it = S.find(v);
		if(it == S.end()) return false;
		S.erase(it);
		return true;
	}
	/// @brief キューの最大元を取り出す
	/// @return キューの最大元
	T get_max() {
		return *S.rbegin();
	}
	T get_min() {
		return *S.begin();
	}
	void pop_max() {
		assert(!S.emtpy()
			   && "キューが空の時にpop_maxが呼ばれました.");
		S.erase(S.rbegin());
	}
	void pop_min() {
		assert(!S.emtpy()
			   && "キューが空の時にpop_minが呼ばれました.");
		S.erase(S.begin());
	}
	int count(T v) {
		auto it = S.lower_bound(v);
		if(it == S.end() || *it != v) {
			return 0;
		}
		return S.upper_bound - it;
	}
	/// @brief キューが要素vを含むかを判定する
	/// @param v キューが含むかを判定する要素
	/// @return キューがvを含むかどうか
	bool contains(T v) {
		return S.find(v) != S.end();
	}
	/// @brief キューの要素数を取得する
	/// @return キューの要素数
	size_t size() {
		return S.size();
	}
};