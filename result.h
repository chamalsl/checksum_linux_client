#ifndef RESULT_H
#define RESULT_H
#include <string>

class Result{
    public:
    enum RESULT_TYPE{
        CORRECT,
        WRONG,
        WARNING,
        EMPTY
    };

    unsigned short m_httpStatus;
    Result::RESULT_TYPE m_resultType;
    std::string m_message;
};
#endif //RESULT_H