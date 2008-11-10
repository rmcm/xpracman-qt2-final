--     $Id$
-- 
--   Copyright 2003 X=X Computer Software Trust
--    		  Kangaroo Ground Australia 3097
-- 
-- 
--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License published by
--  the Free Software Foundation; either version 2, or (at your option)
--  any later version.
--  
--  This software is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--  
--  You should have received a copy of the GNU General Public License
--  along with this software; see the file COPYING.  If not, write to
--  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
--  
--  Report problems and direct all questions to:
--  
--      Rex McMaster, rmcm@compsoft.com.au
--  
-- 

-- ------------------------------------------------------------
-- Perform a multiservice reduction on the items of an invoice.
-- ... the services must be on the same day, and the same invoice,
--     which are the arguments to this function. The reductions
--     as fixed at 100/50/25 ...
-- ------------------------------------------------------------

 create or replace function svpf_ms_adjust(int, text)
 returns integer
 as 'DECLARE
     -- NAME TABLE.ATTRIBUTE%TYPE;
     -- NAME TABLE%ROWTYPE;
         
     tmp_invc__sequence ALIAS for $1;
     tmp_svpf_date_service ALIAS for $2;
     tmp_rec record;
     eol text;
     count integer;
     new_pc integer;
     ts text;
 
     BEGIN
 
       count := 0;
       ts := quote_literal(tmp_svpf_date_service);
 
     FOR tmp_rec in EXECUTE ''
       select    svpf_amount,
                 svpf__sequence
       from      svpf
       where     svpf_invc__sequence = '' || tmp_invc__sequence || ''
         and     date(svpf_date_service) = '' || ts || ''
         and     svpf_serv_code not in ('' ||
                          chr(39) || ''104'' || chr(39) || '','' ||
                          chr(39) || ''105'' || chr(39) ||
                  '')
       order by  svpf_amount desc;''
 
     LOOP
         count := count + 1;
         new_pc := 25;
         if ( count = 1) then
           new_pc := 100;
         end if;
         if ( count = 2) then
           new_pc := 50;
         end if;
 
         update svpf
            set svpf_percentage = new_pc
          where svpf__sequence = tmp_rec.svpf__sequence;
 
     END LOOP;
 
 
     return count - 1;
     END;'
   LANGUAGE 'plpgsql';
 
-- ------------------------------------------------------------
-- Function: Determine the credit paid to a service
-- ------------------------------------------------------------

create or replace function fn_guess_credit(int)
returns  numeric
as 'DECLARE

    x_svpf__sequence ALIAS for $1;
    x_cred_amount NUMERIC;
    x_invc__sequence INTEGER;
    tmp_rec record;

    BEGIN

    -- Determine the invoice and the total amount paid
    select      coalesce(sum(cred_amount), 0.0) + coalesce(sum(cred_gst_amount), 0.0),
                cred_invc__sequence
    into        x_cred_amount,
                x_invc__sequence
    from        cred
    where       cred_invc__sequence = (select   svpf_invc__sequence
                                         from   svpf
                                        where   svpf__sequence = x_svpf__sequence)
    group by cred_invc__sequence;

    if (not found) then
        return 0.0;
    end if;

    -- RAISE NOTICE ''invc = %'', x_invc__sequence;

    FOR tmp_rec in EXECUTE ''
      select    (svpf_amount + svpf_gst_amount) as tmp_svpf_total,
                svpf__sequence
      from      svpf
      where     svpf_invc__sequence = '' || x_invc__sequence || ''
      order by  svpf_date_service, svpf__sequence;''

    LOOP

        -- are we at the specified service?
        if (tmp_rec.svpf__sequence = x_svpf__sequence) then
          if (x_cred_amount > tmp_rec.tmp_svpf_total) then
             return tmp_rec.tmp_svpf_total;
          else
             return x_cred_amount;
          end if;
        end if;
        -- decrement
        x_cred_amount := x_cred_amount - tmp_rec.tmp_svpf_total;
        
        -- nothing left
        if (x_cred_amount < 0.0) then
          return 0.0;
        end if;

    END LOOP;

    RAISE EXCEPTION ''An unexpected error occurred - please report this to your supplier'';

    return 0.0;

    END;'
  LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- Function: Round up to nearest 5 cents
-- ------------------------------------------------------------

create or replace function fn_round_amount(numeric)
returns numeric as $$
    DECLARE
      a_amount ALIAS for $1;
      x_amount numeric;

    BEGIN

      x_amount := ((ceil(a_amount * 20) * 5)::numeric)/100;
      return x_amount;
    END;
$$
LANGUAGE 'plpgsql';

-- update history;
\set mttb_name '\'svpf\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

