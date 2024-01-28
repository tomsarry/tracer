#ifndef COPYABLE_H
#define COPYABLE_H

#include <memory>
template <class T>
class copyable {
   public:
	virtual ~copyable() noexcept = default;

	virtual std::shared_ptr<T> deep_copy() const noexcept = 0;
};

#endif