#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <algorithm>
#include <vector>

template <typename T>
struct matrix
{
    typedef std::size_t     Index;
    typedef std::vector <T> Data;

    Index rows;
    Index columns;
    Data  data;

    matrix(): rows(), columns() { }
    matrix( Index rows, Index columns ): rows(rows), columns(columns), data( rows * columns, T() ) { }

    Index index( Index row = 0, Index column = 0 ) const { return row * columns + column; }

    T&       operator () ( Index row, Index column )       { return data[index( row, column )]; }
    const T& operator () ( Index row, Index column ) const { return data[index( row, column )]; }

    void resize( Index rows, Index columns )
    {
        matrix m( rows, columns );
        for (Index row = 0; row < std::min( rows,    this->rows    ); ++row)
            for (Index col = 0; col < std::min( columns, this->columns ); ++col)
                m( row, col ) = operator () ( row, col );
        data.swap( m.data );
        this->rows    = rows;
        this->columns = columns;
    }

    void clear()
    {
        rows = columns = 0;
        data.clear();
    }
};

#endif 