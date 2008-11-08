-- -*-sql-*-
--      $Id: short_date.sql 843 2001-09-30 08:38:10Z source $ 
--
--    Copyright 2000 X=X Computer Software Trust 
--                  Kangaroo Ground Australia 3097 
--
--
--   This is free software; you can redistribute it and/or modify 
--   it under the terms of the GNU General Public License published by 
--   the Free Software Foundation; either version 2, or (at your option) 
--   any later version. 
--
--   This software is distributed in the hope that it will be useful, 
--   but WITHOUT ANY WARRANTY; without even the implied warranty of 
--   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
--   GNU General Public License for more details. 
--
--   You should have received a copy of the GNU General Public License 
--   along with this software; see the file COPYING.  If not, write to 
--   the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
--
--   Report problems and direct all questions to: 
--
--       Rex McMaster, rmcm@compsoft.com.au 
--
-- General functions associated with any database
--


-- ------------------------------------------------------------
-- Convert timestamp to standard short date string
-- ------------------------------------------------------------

drop function short_date(timestamp);
create function short_date(timestamp)
returns text
as 'DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    arg_ts alias for $1;

    BEGIN

        return to_char(arg_ts,''DD-MM-YYYY'');
      
    END;'
  LANGUAGE 'plpgsql';
