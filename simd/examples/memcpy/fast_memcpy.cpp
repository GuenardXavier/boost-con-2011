#include <nt2/sdk/simd/pack.hpp>
#include <nt2/sdk/memory/align_on.hpp>
#include <nt2/sdk/memory/is_aligned.hpp>

#include <nt2/include/timing.hpp>

template<typename T>
void* naive_memcpy(T* out, T const* in, std::size_t sz)
{
    for(std::size_t i=0; i != sz; ++i)
        out[i] = in[i];
        
    return out;
}

template<ssize_t Offset, typename T>
void* fast_memcpy_aligned(T* out, T const* in, std::size_t sz)
{
    using namespace nt2::memory;
    static const std::size_t N = nt2::meta::native_cardinal<T>::value;
    
    NT2_ASSERT(is_aligned(sz*sizeof(T)) && is_aligned(in + Offset) && is_aligned(out));
    for(std::size_t i=0; i != sz/N; ++i)
        nt2::store(nt2::load<nt2::simd::pack<T>, Offset>(in, i), out, i);
        
    return out;
}

template<typename T>
void* fast_memcpy_aligned(ssize_t offset, T* out, T const* in, std::size_t sz)
{
    #define M0(z,n,t) case n:  return fast_memcpy_aligned<n> (out, in, sz);     \
                      case -n: return fast_memcpy_aligned<-n>(out, in, sz);
    switch(offset)
    {
        case 0:
            return fast_memcpy_aligned<0>(out, in, sz);
            
        BOOST_PP_REPEAT_FROM_TO(1, 32, M0, ~)
        
        default:
            return naive_memcpy(out, in, sz);
    }
    #undef M0
}

template<typename T>
void* fast_memcpy_aligned(T* out, T const* in, std::size_t sz)
{
    return fast_memcpy_aligned<0>(out, in, sz);
}

template<typename T>
void* fast_memcpy(T* out, T const* in, std::size_t sz)
{
    using namespace nt2::memory;
    static const std::size_t N = nt2::meta::native_cardinal<T>::value;
    
    T* orig_out = out;
    
    std::size_t in_diff  = align_on(in)  - in;
    std::size_t out_diff = align_on(out) - out;
    
    naive_memcpy(out, in, out_diff);
    out += out_diff;
    in  += out_diff;
    sz  -= out_diff;
    
    ssize_t offset = out_diff - in_diff;
    std::size_t aligned = sz / N * N;
    fast_memcpy_aligned(
        offset,
        out, in-offset, aligned
    );
    out += aligned;
    in  += aligned;
    sz  -= aligned;
    
    naive_memcpy(out, in, sz);
    return orig_out;
}

int main()
{
    const std::size_t N = 1024*1024+42;
    void* p1 = std::malloc(N + 100);
    void* p2 = std::malloc(N + 100);
    
    char* buffer_in  = (char*)p1 + 11;
    char* buffer_out = (char*)p2 + 5;
    
    for(std::size_t i=0; i!=N; ++i)
        buffer_in[i] = char(i);
    
    nt2::ctic();
    std::memcpy(buffer_out, buffer_in, N);
    std::cout << "std::memcpy: " << (nt2::ctoc(false) / (N*1.)) << std::endl;
    
    for(std::size_t i=0; i!=N; ++i)
    {
        if(buffer_out[i] != char(i))
        {
            std::cout << "Invalid!" << std::endl;
            break;
        }
    }
    
    nt2::ctic();
    fast_memcpy(buffer_out, buffer_in, N);
    std::cout << "fast_memcpy: " << (nt2::ctoc(false) / (N*1.)) << std::endl;
    
    for(std::size_t i=0; i!=N; ++i)
    {
        if(buffer_out[i] != char(i))
        {
            std::cout << "Invalid!" << std::endl;
            break;
        }
    }
}
