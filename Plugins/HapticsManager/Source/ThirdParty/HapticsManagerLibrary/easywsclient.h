#ifndef EASYWSCLIENT_HPP_20120819_MIOFVASDTNUASZDQPLFD
#define EASYWSCLIENT_HPP_20120819_MIOFVASDTNUASZDQPLFD

// This code comes from:
// https://github.com/dhbaird/easywsclient
//
// To get the latest version:
// wget https://raw.github.com/dhbaird/easywsclient/master/easywsclient.hpp
// wget https://raw.github.com/dhbaird/easywsclient/master/easywsclient.cpp

#include <string>
#include <vector>

namespace easywsclient {

	struct CallbackImp
	{
		virtual void operator()(const std::string& message) = 0;
	};
	struct BytesCallbackImp
	{
		virtual void operator()(const std::vector<uint8_t>& message) = 0;
	};

	struct CharCallbackImp
	{
		virtual void operator()(const char* message) = 0;
	};

	class WebSocket {
	public:
		typedef WebSocket * pointer;
		typedef enum readyStateValues { CLOSING, CLOSED, CONNECTING, OPEN } readyStateValues;

		// Factories:
		static pointer create(const std::string &host, int port, const std::string &path);
		// Interfaces:
		virtual ~WebSocket() { }
		virtual void poll(int timeout = 0) = 0; // timeout in milliseconds
		virtual void send(const std::string& message) = 0;
		virtual void sendBinary(const std::string& message) = 0;
		virtual void sendBinary(const std::vector<uint8_t>& message) = 0;
		virtual void sendPing() = 0;
		virtual void close() = 0;
		virtual readyStateValues getReadyState() const = 0;

		template<class Callable>
		void dispatch(Callable callable)
			// For callbacks that accept a string argument.
		{ // N.B. this is compatible with both C++11 lambdas, functors and C function pointers
			struct _Callback : public CallbackImp {
				Callable& callable;
				_Callback(Callable& callable) : callable(callable) { }
				void operator()(const std::string& message) { callable(message); }
			};
			_Callback callback(callable);
			_dispatch(callback);
		}

		template<class Callable>
		void dispatchChar(Callable callable)
			// For callbacks that accept a Char argument.
		{ // N.B. this is compatible with both C++11 lambdas, functors and C function pointers
			struct _Callback : public CharCallbackImp {
				Callable& callable;
				_Callback(Callable& callable) : callable(callable) { }
				void operator()(const char* message) { callable(message); }
			};
			_Callback callback(callable);
			_dispatchChar(callback);
		}

		template<class Callable>
		void dispatchBinary(Callable callable)
			// For callbacks that accept a std::vector<uint8_t> argument.
		{ // N.B. this is compatible with both C++11 lambdas, functors and C function pointers
			struct _Callback : public BytesCallbackImp {
				Callable& callable;
				_Callback(Callable& callable) : callable(callable) { }
				void operator()(const std::vector<uint8_t>& message) { callable(message); }
			};
			_Callback callback(callable);
			_dispatchBinary(callback);
		}

	protected:
		virtual void _dispatch(CallbackImp& callable) = 0;
		virtual void _dispatchBinary(BytesCallbackImp& callable) = 0;
		virtual void _dispatchChar(CharCallbackImp& callable) = 0;
	};

} // namespace easywsclient

#endif /* EASYWSCLIENT_HPP_20120819_MIOFVASDTNUASZDQPLFD */