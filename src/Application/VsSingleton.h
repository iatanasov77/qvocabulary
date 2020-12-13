template <typename T, typename D = T>
class VsSingleton
{
    friend D;
    static_assert(std::is_base_of_v<T, D>, "T should be a base type for D");

public:
    static T& instance();

private:
    VsSingleton() = default;
    ~VsSingleton() = default;
    VsSingleton( const VsSingleton& ) = delete;
    VsSingleton& operator=( const VsSingleton& ) = delete;
};

template <typename T, typename D>
T& VsSingleton<T, D>::instance()
{
    static D inst;
    return inst;
}
