#include "Singleton.h"

class example : public Singleton<example>
{
public:
	friend class Singleton<example>; // Singleton でのインスタンス作成は許可

public:
	int public_member;

protected:
	example(); // 外部でのインスタンス作成は禁止
	virtual ~example();
};

example::example()
{
}

example::~example()
{
}
