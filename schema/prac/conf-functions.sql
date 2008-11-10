-- -*-sql-*-
--      $Id$ 
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
-- General functions associated with the invc table
--

--
-- PL function which returns the invoice balance from the fields
-- in the invc table - no update of recalculation from svpf/cred tables.
-- is performed.
--


-- ------------------------------------------------------------
-- Return the integer value of a configuration variable.
-- ------------------------------------------------------------

drop function conf_integer(text);
create function conf_integer(text)
returns integer
as 'DECLARE
    tmp_conf_code ALIAS for $1;
    value integer;

    BEGIN

    select  conf_value
    into    value
    from    conf
    where   conf_code = tmp_conf_code;

    if ( not found ) then
      RAISE EXCEPTION ''Not Found: %'', tmp_conf_code;
      return null;
    end if;

    return value;
      
    END;'
  LANGUAGE 'plpgsql';

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
