#ifndef ZIP_LONGEST_HPP
#define ZIP_LONGEST_HPP

#include <boost/optional.hpp>
#include <tuple>
#include <utility>
#include "iterator_range.hpp"

namespace iter {
    template <typename ... Containers>
        struct zip_longest_iter;
    template <typename ... Containers>
        iterator_range<zip_longest_iter<Containers...>>
        zip_longest(Containers & ... containers)
        {
            auto begin = 
                zip_longest_iter<Containers...>(containers...);
            auto end = 
                zip_longest_iter<Containers...>(containers...);
            return iterator_range<decltype(begin)>(begin,end);
        }   
    /*
    template <int N,typename Tuple>
    auto zip_get(Tuple & t)->decltype(*std::get<N>(t))&
    {
        return *std::get<N>(t);
    }
    */
    template <typename Container>
        struct zip_longest_iter<Container> {
        public:
            using Iterator = decltype(std::declval<Container>().begin());
        private:
            Iterator begin;
            const Iterator end;

        public:
            zip_longest_iter(Container & c) :
                begin(c.begin()),end(c.end()) {}

            std::tuple<boost::optional<Iterator>> operator*()     
            {
                return std::make_tuple(begin != end ? 
                        boost::optional<Iterator>(begin)
                        : boost::optional<Iterator>());
            }
            zip_longest_iter & operator++() {
                if(begin!=end)++begin;
                return *this;
            }
            bool operator!=(const zip_longest_iter &) const {
                return begin != end;
            }
        };
    template <typename Container, typename ... Containers>
        struct zip_longest_iter<Container,Containers...> {
        public:
            using Iterator = decltype(std::declval<Container>().begin());
        private:
            Iterator begin;
            const Iterator end;
            zip_longest_iter<Containers...> inner_iter;
            
        public:
            using Elem_t = decltype(*begin);
            using tuple_t = 
                decltype(std::tuple_cat(std::tuple<boost::optional<Iterator>>(),*inner_iter));

            zip_longest_iter(Container & c, Containers & ... containers) :
                begin(c.begin()),
                end(c.end()),
                inner_iter(containers...) {}

            //this is for returning a tuple of optional<iterator>

            tuple_t operator*()
            {
                return std::tuple_cat(std::make_tuple(begin != end ?boost::optional<Iterator>(begin):boost::optional<Iterator>()),*inner_iter);
            }
            zip_longest_iter & operator++() {
                if (begin != end) ++begin;
                ++inner_iter;
                return *this;
            }
            bool operator!=(const zip_longest_iter & rhs) const {
                return begin != end || (this->inner_iter != rhs.inner_iter);
            }
        };
}
//should add reset after the end of a range is reached, just in case someone 
//tries to use it again
//this means it's only safe to use the range ONCE
#endif //ZIP_LONGEST_HPP
