#include "daylight/base.hpp"
template<typename S>
struct SWAG {
private:
	function<S(S, S)> op;
	struct node {
		S value, sum;
		node(const S &value, const S &sum)
			: value(value), sum(sum) {
		}
	};
	stack<node> front_t, back_t;

	void move() {
		assert(!back_t.empty());
		front_t.emplace(back_t.top().value,
						back_t.top().value);
		back_t.pop();
		while(!back_t.empty()) {
			front_t.emplace(
				back_t.top().value,
				op(back_t.top().value, front_t.top().sum));
			back_t.pop();
		}
	}

public:
	/// @brief コンストラクタ
	/// @param op 半群の演算
	SWAG(const function<S(S, S)> &op): op(op) {
	}
	/// @brief SWAGに要素を追加
	/// @param s 追加する要素
	void push(const S &s) {
		if(back_t.empty())
			back_t.emplace(s, s);
		else
			back_t.emplace(s, op(s, back_t.top().sum));
	}
	/// @brief SWAGから要素を削除
	void pop() {
		if(front_t.empty()) {
			move();
		}
		front_t.pop();
	}
	/// @brief SWAGの先頭要素にアクセス
	/// @return 先頭要素の値
	S front() {
		if(front_t.empty()) {
			move();
		}
		return front_t.top().value;
	}
	/// @brief SWAG内の要素の総積を求める
	/// @return 要素の総積
	S fold() const {
		assert(!empty());
		if(front_t.empty()) {
			return back_t.top().sum;
		} else if(back_t.empty()) {
			return front_t.top().sum;
		} else {
			return op(front_t.top().sum, back_t.top().sum);
		}
	}
	/// @brief SWAGが空か判定する
	/// @return SWAGが空ならtrue
	bool empty() const {
		return front_t.empty() && back_t.empty();
	}
	/// @brief SWAG内の要素の数を取得する
	/// @return SWAG内の要素の数
	size_t size() {
		return SZ(front_t) + SZ(back_t);
	}
	/// @brief SWAG内の全要素を削除する
	void clear() {
		front_t = stack<node>();
		back_t = stack<node>();
	}
};