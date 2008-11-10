-- -*-sql-*-
--      $Id$ 
--
--    Copyright 2001 X=X Computer Software Trust 
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

drop function get_paym_total(integer);
create function get_paym_total(integer)
returns numeric
as 'DECLARE
        tmp_paym_number alias for $1;
        tmp_paym_total paym.paym_amount%TYPE;

    BEGIN

    
      select coalesce( sum(cred_amount + cred_gst_amount), ''0.00''::numeric )
      into   tmp_paym_total
      from   cred
      where  cred_paym__sequence = tmp_paym_number;

      return tmp_paym_total;

    END;'
    LANGUAGE 'plpgsql';

drop function set_paym_total(integer);
create function set_paym_total(integer)
returns numeric
as 'DECLARE
        tmp_paym_number alias for $1;
        tmp_paym_total paym.paym_amount%TYPE;

    BEGIN

      tmp_paym_total = get_paym_total( tmp_paym_number );
      update  paym
      set     paym_amount = tmp_paym_total
      where   paym__sequence = tmp_paym_number;

      return tmp_paym_total;

    END;'
    LANGUAGE 'plpgsql';

