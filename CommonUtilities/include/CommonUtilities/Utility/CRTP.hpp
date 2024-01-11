#pragma once

template<typename T, template<typename> class CRTPType>
class CRTP
{
public:
	T& Underlying();
	const T& Underlaying() const;

private:
	CRTP(); // force derived to inherit from the correct CRTP
	friend CRTPType<T>;
};

template<typename T, template<typename> class CRTPType>
inline CRTP<T, CRTPType>::CRTP() = default;

template<typename T, template<typename> class CRTPType>
inline T& CRTP<T, CRTPType>::Underlying()
{
	return static_cast<T&>(*this);
}
template<typename T, template<typename> class CRTPType>
inline const T& CRTP<T, CRTPType>::Underlaying() const
{
	return static_cast<const T&>(*this);
}
