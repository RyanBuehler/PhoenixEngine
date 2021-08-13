#pragma once

class IBindable
{
public:

  IBindable() noexcept {};
  virtual ~IBindable() noexcept {};

  virtual void Bind() noexcept = 0;

private:

};
