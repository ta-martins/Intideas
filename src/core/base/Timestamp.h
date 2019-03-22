#pragma once
#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>

typedef std::chrono::high_resolution_clock HRClock;

class Timestamp
{
public:
	Timestamp() = default;
	~Timestamp() = default;

	inline void setTimeZeroPointNow();
	inline HRClock::time_point estimateTimePointNow();

	inline long long apiTimeStamp_ns() const;
	inline double apiTimeStamp_us() const;
	inline double apiTimeStamp_ms() const;
	inline double apiTimeStamp_s()  const;

	inline long double hardwareTimeStamp_ns(long double dt) const;
	inline double hardwareTimeStamp_us(long double dt) const;
	inline double hardwareTimeStamp_ms(long double dt) const;
	inline double hardwareTimeStamp_s(long double dt) const;

	static inline std::string apiTimeSystemClockString();
	static inline std::string apiUptimeString();

private:
	HRClock::time_point timePoint;
	static const std::chrono::system_clock::time_point start;
};

inline void Timestamp::setTimeZeroPointNow()
{
	timePoint = HRClock::now();
}

inline HRClock::time_point Timestamp::estimateTimePointNow()
{
	//requires implementation
	return HRClock::time_point();
}

inline long long Timestamp::apiTimeStamp_ns() const
{
	return (HRClock::now() - timePoint).count();
}

inline double Timestamp::apiTimeStamp_us() const
{
	return std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(HRClock::now() - timePoint).count();
}

inline double Timestamp::apiTimeStamp_ms() const
{
	return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(HRClock::now() - timePoint).count();
}

inline double Timestamp::apiTimeStamp_s() const
{
	return std::chrono::duration_cast<std::chrono::duration<double>>(HRClock::now() - timePoint).count();
}

inline long double Timestamp::hardwareTimeStamp_ns(long double dt) const
{
	return 0;
}

inline double Timestamp::hardwareTimeStamp_us(long double dt) const
{
	return 0;
}

inline double Timestamp::hardwareTimeStamp_ms(long double dt) const
{
	return 0;
}

inline double Timestamp::hardwareTimeStamp_s(long double dt) const
{
	return 0;
}

inline std::string Timestamp::apiTimeSystemClockString()
{
	time_t tn = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	tm utc;
	gmtime_s(&utc, &tn);

	std::stringstream ss;

	ss << utc.tm_year + 1900 << "-" << utc.tm_mon + 1 << "-" << utc.tm_mday << " " << utc.tm_hour << ":" << utc.tm_min << ":" << (utc.tm_sec < 10 ? "0" : "") << utc.tm_sec;

	std::string result = ss.str();

	return result;
}

inline std::string Timestamp::apiUptimeString()
{
	std::chrono::system_clock::time_point sctp = std::chrono::system_clock::now();
	std::chrono::system_clock::duration tp = sctp - start;

	typedef std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<24>>::type> days;
	
	days d = std::chrono::duration_cast<days>(tp);
	tp -= d;
	std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(tp);
	tp -= h;
	std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(tp);
	tp -= m;
	std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(tp);
	tp -= s;

	std::stringstream ss;

	ss << d.count() << " days " << h.count() << " hours " << m.count() << " minutes " << s.count() << " seconds";

	std::string result = ss.str();

	return result;
}
