#pragma once

class Any {
 private:
  class InnerBase {
   public:
    virtual InnerBase* Clone() const {
      return nullptr;
    }
    virtual ~InnerBase() {}
  };

  template<class T>
  class Inner : public InnerBase {
   friend class Any;
   public:
    virtual InnerBase* Clone() const {
      return new Inner(value_);
    }
    Inner(const T &value) : value_(value) {}
   private:
    T value_;
  };

  std::unique_ptr<InnerBase> value_ = nullptr;

 public:
  Any() {
  }

  template<class T>
  Any(const T &value) {
      value_ = std::unique_ptr<InnerBase>(new Inner<T>(value));
  }

  template<class T>
  Any &operator=(const T & value) {
    this->value_ = std::make_unique<Inner<T>>(value);
    return *this;
  }

  Any(const Any &rhs) {
    value_ = std::unique_ptr<InnerBase>(rhs.value_->Clone());
  }

  Any &operator=(Any rhs) {
    value_.swap(rhs.value_);
    return *this;
  }

  ~Any() {
    this->value_ = nullptr;
  }

  bool Empty() const {
    return this->value_ == nullptr;
  }

  void Clear() {
    this->~Any();
  }

  void Swap(Any &rhs) {
    value_.swap(rhs.value_);
  }

  template<class T>
  const T &GetValue() {
    return (dynamic_cast<const Inner<T> &>(*value_)).value_;
  }
};
