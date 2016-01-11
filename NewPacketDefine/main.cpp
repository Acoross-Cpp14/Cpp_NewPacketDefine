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
	/*입력*/	\
	struct In \
		InArgs	\
	in_;	\
	/*리스폰스*/	\
	struct Out	\
		OutArgs	\
	;	\
	/*기본생성자*/	\
	Name() {}	\
	/*초기화생성자*/	\
	template <class... Args>	\
	Name(Args&&... args){ in_ = { std::forward<Args>(args)... }; }	\
	\
	/*리스폰스 콜백 타입*/	\
	typedef std::function<int(Out& out, int err)> CallbackT;	\
	/*동기 요청*/	\
	Out Request(int timeout);	\
	/*비동기 요청*/	\
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
	// packet class 와 동일한 이름의 enum 이 생성된다.
	RQ_Packet::RQ_VERSION;

	// 사용법1. packet object 를 생성하고 맴버함수로 AsyncRequest 호출한다.
	RQ_VERSION packet1;
	packet1.in_.version = 1;
	packet1.in_.count = 100;
	auto out = packet1.AsyncRequest(0, [](RQ_VERSION::Out& out, int err)->int
	{
		return 0;
	});

	// 생성자에 인자를 넘길 수 있다.
	RQ_VERSION packet2(1, 100);
	auto out = packet2.AsyncRequest(0, [](RQ_VERSION::Out& out, int err)->int
	{
		return 0;
	});

	// 사용법2. 전역함수 AsyncRequest 를 이용한다. 
	// 함수 호출 시 packet 에 대한 인자를 넘긴다.
	int ret = AsyncRequest<RQ_VERSION>(0, 
		[](RQ_VERSION::Out& out, int err)->int
		{
			return 0;
		},
		1, 100
	);

	// callback 함수는 nullptr 일 수 있다. 이 경우 response 를 처리하지 않는다. 
	ret = AsyncRequest<RQ_VERSION>(0, nullptr, 
		1, 100
		);

	system("pause");

	return 0;
}