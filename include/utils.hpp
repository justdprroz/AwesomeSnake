template <typename T> 
T getRandomReal(T a, T b) 
{
    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::uniform_real_distribution<T> distrib(a, b);
    return distrib(randomGenerator);
}

struct vec2f {
    vec2f() = default;
    vec2f(const float _x, const float _y) : x(_x), y(_y) {};
    vec2f(const float _x) : x(_x), y(_x) {};
    float x;
    float y;
    bool operator==(vec2f other) {
        return (this->x == other.x && this->y == other.y);
    }
    bool operator!=(vec2f other) {
        return (this->x != other.x || this->y != other.y);
    }
    void operator+=(vec2f other) {
        this->x += other.x;
        this->y += other.y;
    }
    vec2f operator-(vec2f other) {
        return vec2f(this->x - other.x, this->y - other.y);
    }
    vec2f operator+(vec2f other) {
        return vec2f(this->x + other.x, this->y + other.y);
    }
    vec2f operator*(float num) {
        return vec2f(this->x * num, this->y * num);
    }
    vec2f operator*(int num) {
        return vec2f(this->x * num, this->y * num);
    }
    vec2f operator*(double num) {
        return vec2f(this->x * num, this->y * num);
    }
};

template <typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}