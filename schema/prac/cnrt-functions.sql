--  -*- sql -*-
--  
--    $Id$

--  Copyright 2001 X=X Computer Software Trust
--   		  Kangaroo Ground Australia 3097


-- This is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public Licensepublished by
-- the Free Software Foundation; either version 2, or (at your option)
-- any later version.
 
-- This software is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
 
-- You should have received a copy of the GNU General Public License
-- along with this software; see the file COPYING.  If not, write to
-- the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 
-- Report problems and direct all questions to:
 
--     Rex McMaster, rmcm@compsoft.com.au
 


-- ------------------------------------------------------------
-- Post a debit to svpf if applicable
-- ------------------------------------------------------------

drop function cnrt_post_debit(integer);
create function cnrt_post_debit(integer)
returns integer
as 'DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;

    arg_patn__sequence ALIAS for $1;
    cnrt_row cnrt%ROWTYPE;
    count_post integer;
    amount_posted svpf.svpf_amount%TYPE;
    tmp_desc text;

    BEGIN

        count_post := 0;

        -- find the due postings
        --

        FOR cnrt_row IN

          select    *
          from      cnrt
          where     cnrt_patn__sequence = arg_patn__sequence
          and       cnrt_balance > 0
          and       ( cnrt_last_date + ctrt_period < ''today''::timestamp
                      or cnrt_last_date is null)

        LOOP

          if (cnrt_row.cnrt_last_date is null ) then
            amount_posted := cnrt_row.cnrt_first_installment;
          else
            amount_posted := cnrt_row.cnrt_other_installment;
          end if;
          tmp_desc := ''Installment - '' || to_char(''now''::timestamp, ''Month'');

          -- check for overposting
          if ( amount_posted > cnrt_row.cnrt_balance ) then
            amount_posted := cnrt_row.cnrt_balance;
          end if;

          -- post the installment
          -- raise notice ''%/%/%/%'', cnrt_row.cnrt_patn__sequence, cnrt_row.cnrt_serv_code,tmp_desc, amount_posted;
          insert into svpf(
                svpf_date_service,
                svpf_serv_code,
                svpf_desc,
                svpf_amount,
                svpf_patn__sequence)
            values(
                ''now''::timestamp,
                cnrt_row.cnrt_serv_code,
                tmp_desc,
                amount_posted,
                cnrt_row.cnrt_patn__sequence);

          -- update the contract

          update cnrt
          set    cnrt_balance = cnrt_balance - amount_posted,
                 cnrt_last_date = ''today''::timestamp
          where  cnrt__sequence = cnrt_row.cnrt__sequence;
          count_post := count_post + 1;

        END LOOP;

      

    return count_post;
    END;'
  LANGUAGE 'plpgsql';
