--     $Id$
-- 
--   Copyright 2005 X=X Computer Software Trust
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
-- Function: Create a Claim from an invoice
-- ------------------------------------------------------------

drop function fn_create_hpcl_batch(integer);
create function fn_create_hpcl_batch(integer)
returns integer
as 'DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;

        x_invc__sequence ALIAS for $1;
        tmp_invc invc%ROWTYPE;
        tmp_svpf_count integer;
        tmp_hpcl__sequence hpcl.hpcl__sequence%TYPE;

    BEGIN

        -- check for the invoice.

        select  *
        into    tmp_invc
        from    invc
        where   invc__sequence = x_invc__sequence
        and     not exists (select hpcl_invc__sequence from hpcl where hpcl_invc__sequence = x_invc__sequence);

        if (not found) then
          RAISE EXCEPTION ''An unclaimed invoice number % not found'', x_invc__sequence;
          return null;
        end if;

        -- count the services

        select  count(*)
        into    tmp_svpf_count
        from    svpf
        where   svpf_invc__sequence = x_invc__sequence;

        if (tmp_svpf_count < 1) then
          RAISE EXCEPTION ''There are no services to be claimed for this invoice - % '', x_invc__sequence;
          return null;
        end if;

        -- create a claim voucher row in HPCL

        select  nextval(''hpcl_hpcl__sequence_seq'')
        into    tmp_hpcl__sequence;
        insert into hpcl(hpcl__sequence, hpcl_patn__sequence, hpcl_invc__sequence, hpcl_prov_code, hpcl_rfdr_code)
                values(tmp_hpcl__sequence, tmp_invc.invc_patn__sequence, x_invc__sequence, tmp_invc.invc_prov_code, tmp_invc.invc_rfdr_code);

        -- make sure the invoice is closed.
        update  invc
        set     invc_date_printed = now()
        where   invc__sequence = x_invc__sequence
        and     invc_date_printed is null;

    return tmp_hpcl__sequence;

    END;'
  LANGUAGE 'plpgsql';

