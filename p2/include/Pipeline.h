#ifndef __Pipeline_h
#define __Pipeline_h

// OVERVIEW: Pipeline.h
// ========
// Class definition for general processing pipeline.
//
// Authors: Arthur de Andrade, João de Carvalho e Nathan Rezende
// Last revision: 16/06/2026

#include <utility>
#include <tuple>
#include <concepts>
#include <stdexcept>

namespace tcii::cg
{ // begin namespace tcii::cg

/// @brief | operator overload for chaining functions
template <typename T, typename F>
requires std::invocable<F, T>
auto operator | (T&& val, F&& f)
{
    return std::forward<F>(f)(std::forward<T>(val));
}

/// @brief Processing pipeline for general pourposes
/// @tparam ...StageFuncs 
template <typename... StageFuncs>
class Pipeline
{
public:
    explicit Pipeline(StageFuncs... funcs):
        _stages(std::move(funcs)...)
    { /* Do nothing */ }

    template <typename T>
    auto execute(T&& initialInput)
    {
        return execute_impl(std::forward<T>(initialInput), std::make_index_sequence<sizeof...(StageFuncs)>{});
    }

private:
    std::tuple<StageFuncs...> _stages;

    /// @brief Left fold expression to apply each stage function in sequence
    template <typename T, size_t... Is>
    auto execute_impl(T&& input, std::index_sequence<Is...>)
    {
        return (std::forward<T>(input) | ... | std::get<Is>(_stages));
    }
};

template <typename... StageFuncs>
Pipeline(StageFuncs...) -> Pipeline<StageFuncs...>;

} // end namespace tcii::cg
#endif // __Pipeline_h