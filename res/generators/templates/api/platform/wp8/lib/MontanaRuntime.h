#pragma once

#include "../../../../shared/generated/cpp/I<%= $cur_module.name %>.h"
#include "api_generator/wp8/IMethodResult.h"
<%
  dynamic_methods = ''
  static_methods = ''

  $cur_module.methods.each do |module_method|
    next if !module_method.generateNativeAPI

    params = ''
    call_params = ''
    module_method.params.each do |param|
        params += "#{api_generator_cli_makeNativeTypeArg(param.type)} #{param.name}, "
        call_params += "#{param.name}, "
    end

    params += "IMethodResult^ oResult"
    call_params += 'oResult'
    module_method.cached_data["cli_params"] = params
    module_method.cached_data["cli_call_params"] = call_params

    method_def = "        void #{module_method.native_name}(#{params});\n"
    if module_method.access == ModuleMethod::ACCESS_STATIC
      static_methods += method_def
    else
      dynamic_methods += method_def
    end
  end
  dynamic_methods = "    public:\n" + dynamic_methods if dynamic_methods.length() > 0
  static_methods = "    public:\n" + static_methods if static_methods.length() > 0
%>
namespace rhoruntime
{
    public interface class I<%= $cur_module.name %>Impl
    {
<%= dynamic_methods%>    };

    public interface class I<%= $cur_module.name %>SingletonImpl
    {
<%= static_methods%>    };
    public interface class I<%= $cur_module.name %>FactoryImpl
    {
    public:
        I<%= $cur_module.name %>Impl^ getImpl();
        I<%= $cur_module.name %>SingletonImpl^ getSingletonImpl();
    };

    public ref class <%= $cur_module.name %>RuntimeComponent sealed: public I<%= $cur_module.name %>Impl
    {
    public:
        <%= $cur_module.name %>RuntimeComponent(I<%= $cur_module.name %>Impl^ impl);
<% $cur_module.methods.each do |module_method|
    next if module_method.access == ModuleMethod::ACCESS_STATIC
    next if !module_method.generateNativeAPI
%>        virtual void <%= module_method.native_name%>(<%= module_method.cached_data["cli_params"]%>);
<% end %>    private:
        I<%= $cur_module.name %>Impl^ _impl;
    };

    public ref class <%= $cur_module.name %>SingletonComponent sealed: public I<%= $cur_module.name %>SingletonImpl
    {
    public:
        <%= $cur_module.name %>SingletonComponent(I<%= $cur_module.name %>SingletonImpl^ impl);
<% $cur_module.methods.each do |module_method|
    next if module_method.access != ModuleMethod::ACCESS_STATIC
%>        virtual void <%= module_method.native_name%>(<%= module_method.cached_data["cli_params"]%>);
<% end %>    private:
        I<%= $cur_module.name %>SingletonImpl^ _impl;
    };

    public ref class <%= $cur_module.name %>FactoryComponent sealed
    {
    public:
        static void setImpl(I<%= $cur_module.name %>FactoryImpl^ impl);
    };
}
