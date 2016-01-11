#include <iostream>
#include <functional>
#include <utility>
#include <thread>

class PacketSender
{
public:
	template <class In>
	static int Request(In& in, int timeout)
	{
		return 0;
	}
};

#define endef
#define PacketList \
	PacketDef({}, RQ_VERSION, {int version; int count; })	\
	endef

#define PacketDef(OutArgs, Name, InArgs) Name,
enum class RQ_Packet
{
	PacketList
	RQ_MAX
};
#undef PacketDef

#define PacketDef(OutArgs, Name, InArgs)	\
class Name	\
{	\
public:	\
	/*�Է�*/	\
	struct In \
		InArgs	\
	in_;	\
	/*��������*/	\
	struct Out	\
		OutArgs	\
	;	\
	/*�⺻������*/	\
	Name() {}	\
	/*�ʱ�ȭ������*/	\
	template <class... Args>	\
	Name(Args&&... args){ in_ = { std::forward<Args>(args)... }; }	\
	\
	/*�������� �ݹ� Ÿ��*/	\
	typedef std::function<int(Out& out, int err)> CallbackT;	\
	/*���� ��û*/	\
	Out Request(int timeout);	\
	/*�񵿱� ��û*/	\
	int AsyncRequest(int timeout, CallbackT func){	\
		return 0;	\
	}	\
};
PacketList;
#undef PacketDef;

template <class T, class... Args>
static int AsyncRequest(int timeout, typename T::CallbackT func, Args&&... args)
{
	if (func == nullptr)
		printf("null!\n");

	T a(std::forward<Args>(args)...);
	return a.AsyncRequest(timeout, func);
}


int main(int argc, const char* argv[])
{
	// packet class �� ������ �̸��� enum �� �����ȴ�.
	RQ_Packet::RQ_VERSION;

	// ����1. packet object �� �����ϰ� �ɹ��Լ��� AsyncRequest ȣ���Ѵ�.
	RQ_VERSION packet1;
	packet1.in_.version = 1;
	packet1.in_.count = 100;
	auto out = packet1.AsyncRequest(0, [](RQ_VERSION::Out& out, int err)->int
	{
		return 0;
	});

	// �����ڿ� ���ڸ� �ѱ� �� �ִ�.
	RQ_VERSION packet2(1, 100);
	auto out = packet2.AsyncRequest(0, [](RQ_VERSION::Out& out, int err)->int
	{
		return 0;
	});

	// ����2. �����Լ� AsyncRequest �� �̿��Ѵ�. 
	// �Լ� ȣ�� �� packet �� ���� ���ڸ� �ѱ��.
	int ret = AsyncRequest<RQ_VERSION>(0, 
		[](RQ_VERSION::Out& out, int err)->int
		{
			return 0;
		},
		1, 100
	);

	// callback �Լ��� nullptr �� �� �ִ�. �� ��� response �� ó������ �ʴ´�. 
	ret = AsyncRequest<RQ_VERSION>(0, nullptr, 
		1, 100
		);

	system("pause");

	return 0;
}