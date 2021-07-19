/* ----------------------------------------------------------------------------
  Copyright (c) 2021, Daan Leijen
  This is free software; you can redistribute it and/or modify it
  under the terms of the MIT License. A copy of the license can be
  found in the "LICENSE" file at the root of this distribution.
-----------------------------------------------------------------------------*/
#pragma once
#ifndef RP_STRINGBUF_H
#define RP_STRINGBUF_H

#include "common.h"

//-------------------------------------------------------------
// string buffer
// in-place modified buffer with edit operations 
// that grows on demand.
//-------------------------------------------------------------

// abstract string buffer
struct stringbuf_s;
typedef struct stringbuf_s stringbuf_t;

internal stringbuf_t*  sbuf_new( alloc_t* mem, bool is_utf8 );
internal void    sbuf_free( stringbuf_t* sbuf );
internal char*   sbuf_free_dup(stringbuf_t* sbuf);
internal ssize_t sbuf_len(const stringbuf_t* s);

internal const char* sbuf_string_at( stringbuf_t* sbuf, ssize_t pos );
internal const char* sbuf_string( stringbuf_t* sbuf );
internal char    sbuf_char_at(stringbuf_t* sbuf, ssize_t pos);
internal char*   sbuf_strdup_at( stringbuf_t* sbuf, ssize_t pos );
internal char*   sbuf_strdup( stringbuf_t* sbuf );

// primitive edit operations (inserts return the new position)
internal void    sbuf_clear(stringbuf_t* sbuf);
internal void    sbuf_replace(stringbuf_t* sbuf, const char* s);
internal void    sbuf_delete_at(stringbuf_t* sbuf, ssize_t pos, ssize_t count);
internal void    sbuf_delete_from_to(stringbuf_t* sbuf, ssize_t pos, ssize_t end);
internal ssize_t sbuf_insert_at_n(stringbuf_t* sbuf, const char* s, ssize_t n, ssize_t pos );
internal ssize_t sbuf_insert_at(stringbuf_t* sbuf, const char* s, ssize_t pos );
internal ssize_t sbuf_append_n(stringbuf_t* sbuf, const char* s, ssize_t n);
internal ssize_t sbuf_append(stringbuf_t* sbuf, const char* s);
internal ssize_t sbuf_append_char(stringbuf_t* sbuf, char c);

// high level edit operations (return the new position)
internal ssize_t sbuf_next( stringbuf_t* sbuf, ssize_t pos, ssize_t* cwidth );
internal ssize_t sbuf_prev( stringbuf_t* sbuf, ssize_t pos, ssize_t* cwidth );

internal ssize_t sbuf_delete_char_before( stringbuf_t* sbuf, ssize_t pos );
internal void    sbuf_delete_char_at( stringbuf_t* sbuf, ssize_t pos );
internal ssize_t sbuf_swap_char( stringbuf_t* sbuf, ssize_t pos );

internal ssize_t sbuf_find_line_start( stringbuf_t* sbuf, ssize_t pos );
internal ssize_t sbuf_find_line_end( stringbuf_t* sbuf, ssize_t pos );
internal ssize_t sbuf_find_word_start( stringbuf_t* sbuf, ssize_t pos );
internal ssize_t sbuf_find_word_end( stringbuf_t* sbuf, ssize_t pos );
internal ssize_t sbuf_find_ws_word_start( stringbuf_t* sbuf, ssize_t pos );
internal ssize_t sbuf_find_ws_word_end( stringbuf_t* sbuf, ssize_t pos );

// row/column info
typedef struct rowcol_s {
  ssize_t row;
  ssize_t col;
  ssize_t row_start;
  ssize_t row_len;
  bool    first_on_row;
  bool    last_on_row;
} rowcol_t;

// find row/col position
internal ssize_t sbuf_get_pos_at_rc( stringbuf_t* sbuf, ssize_t termw, ssize_t promptw, ssize_t row, ssize_t col );

// get row/col for a given position
internal ssize_t sbuf_get_rc_at_pos( stringbuf_t* sbuf, ssize_t termw, ssize_t promptw, ssize_t pos, rowcol_t* rc );


// row iteration
typedef bool (row_fun_t)(const char* s,
                          ssize_t row, ssize_t row_start, ssize_t row_len, 
                          bool is_wrap, bool is_utf8, const void* arg, void* res);

internal ssize_t sbuf_for_each_row( stringbuf_t* sbuf, ssize_t termw, ssize_t promptw, row_fun_t* fun, void* arg, void* res );


//-------------------------------------------------------------
// Strings
//-------------------------------------------------------------

// skip a single CSI sequence (ESC [ ...)
internal bool    skip_csi_esc( const char* s, ssize_t len, ssize_t* esclen ); // used in term.c

internal ssize_t str_column_width( const char* s, bool is_utf8 );
internal ssize_t str_next_ofs( const char* s, ssize_t len, ssize_t pos, bool is_utf8, ssize_t* cwidth );
internal const char* str_skip_until_fit( const char* s, ssize_t max_width, bool is_utf8);

// column width of a one utf8 character sequence (of length `n`)
internal ssize_t utf8_char_width( const char* s, ssize_t n );

#endif // RP_STRINGBUF_H