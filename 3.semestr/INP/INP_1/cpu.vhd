-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2023 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Kryštof Michálek xmicha94
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic;                      -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'

   -- stavove signaly
   READY    : out std_logic;                      -- hodnota 1 znamena, ze byl procesor inicializovan a zacina vykonavat program
   DONE     : out std_logic                       -- hodnota 1 znamena, ze procesor ukoncil vykonavani programu (narazil na instrukci halt)
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

  -- CNT 
  signal cnt_inc  : std_logic;
  signal cnt_dec  : std_logic;
  signal cnt_out  : std_logic_vector(3 downto 0); -- random

  -- PTR
  signal ptr_inc  : std_logic;
  signal ptr_dec  : std_logic;
  signal ptr_rst  : std_logic;
  signal ptr_reg  : std_logic_vector(12 downto 0) := (others => '0');

  -- PC
  signal pc_inc   : std_logic;
  signal pc_dec   : std_logic;
  signal pc_reg   : std_logic_vector(12 downto 0) := (others => '0'); 

  -- MX1
  signal mx1_sel  : std_logic;
  signal mx1_out  : std_logic_vector(12 downto 0);

  -- MX2
  signal mx2_sel : std_logic_vector(1 downto 0);
  signal mx2_out : std_logic_vector(7 downto 0);


  -- STAVY PRO FSM
  type fsm_state is (
    -- stavy procesoru
    state_start,
    state_init,
    state_init_1,
    state_init_2,
    state_fetch_01,
    state_fetch,
    state_decode,

    -- while loop
    state_loop_start,
    state_loop_start_1,
    state_loop_start_2,
    state_loop_start_3,
    state_loop_end,
    state_loop_end_1,
    state_loop_end_2,
    state_loop_end_2_1,
    state_loop_end_3,

    -- stavy pro ptr
    state_value_inc,
    state_value_inc_1,
    state_value_inc_2,
    state_value_dec,
    state_value_dec_1,
    state_value_dec_2,
    state_value_rst,

    --stavy pro pc
    state_ptr_inc,
    state_ptr_dec,

    --stav pro ukonceni loopu
    state_break,
    state_break_1,
    state_break_2,

    --stavy pro cteni a zapis
    state_read,
    state_read_1,
    state_write,
    state_write_1,

    -- stav pro ukonceni programu
    state_null
  );
  
  signal state : fsm_state := state_start;
  signal next_state: fsm_state;

begin
  
  -- process pro cnt
  cnt: process (CLK, RESET) is
  begin
    if RESET = '1' then
      cnt_out <= (others => '0');
    elsif rising_edge(CLK) then
      if cnt_inc = '1' then
        cnt_out <= cnt_out + 1;
      elsif cnt_dec = '1' then
        cnt_out <= cnt_out - 1;
      end if;
    end if;
  end process;

  -- process pro ptr
  ptr: process (CLK, RESET) is
  begin
    if RESET = '1' then 
      ptr_reg <= (others => '0');
    elsif rising_edge(CLK) then
      if ptr_rst = '1' then
        ptr_reg <= (others => '0');
      elsif ptr_inc = '1' then
        if ptr_reg = X"1FFF" then
          ptr_reg <= (others => '0');
        else
          ptr_reg <= ptr_reg + 1;
        end if;
      elsif ptr_dec = '1' then
        if ptr_reg = "00000000000000" then
            ptr_reg <= "1111111111111";
        else
          ptr_reg <= ptr_reg - 1;
        end if;
      end if;
    end if;
  end process;

  -- process pro pc
  pc: process (CLK, RESET) is
  begin
    if RESET = '1' then
      pc_reg <= (others => '0');
    elsif rising_edge(CLK) then
      if pc_inc = '1' then
        pc_reg <= pc_reg + 1;
      elsif pc_dec = '1' then
        pc_reg <= pc_reg - 1;
      end if;
    end if;
  end process;

  -- process pro mx1
  mx1: process (CLK, RESET) is
  begin
    if RESET = '1' then
      mx1_out <= (others => '0');
    elsif rising_edge(CLK) then
      if mx1_sel = '0' then
        mx1_out <= ptr_reg;
      else
        mx1_out <= pc_reg;
      end if;
    end if;
  end process;
  
  mx1_lol: process (mx1_out) is
  begin
    DATA_ADDR <= mx1_out;
  end process;
        
  -- process pro mx2
  mx2: process (CLK, RESET) is
  begin
    if RESET = '1' then
      mx2_out <= (others => '0');
    elsif rising_edge(CLK) then
      case mx2_sel is 
        when "00" =>
          mx2_out <= IN_DATA;
        when "01" =>
          mx2_out <= DATA_RDATA - 1;
        when "10" =>
          mx2_out <= DATA_RDATA + 1;
        when others =>
          mx2_out <= (others => '0');
      end case;
    end if;
  end process;

  mx2_lol: process (mx2_out) is
  begin
    DATA_WDATA <= mx2_out;
  end process;

  -- FSM
  next_state_logic: process (CLK, RESET, EN) is
  begin
    if RESET = '1' then
      state <= state_start;
    elsif rising_edge(CLK) and EN = '1' then
        state <= next_state;
    end if;
  end process; 

  fsm: process (state, IN_VLD, OUT_BUSY, DATA_RDATA) is
  begin

    cnt_inc <= '0';
    cnt_dec <= '0';

    ptr_inc <= '0';
    ptr_dec <=  '0';
    ptr_rst <= '0';

    pc_inc <= '0';
    pc_dec <= '0';

    DATA_RDWR <= '0';
    DATA_EN <= '0';

    IN_REQ <= '0';
    OUT_WE <= '0';

    mx2_sel <= "11";

    case state is

      -- start
      when state_start =>
        READY <= '0';
        DONE <= '0';
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        next_state <= state_init;

      -- init
      when state_init =>
        if DATA_RDATA = X"40" then
          ptr_inc <= '1';  
          READY <= '1';
          next_state <= state_fetch_01;  
        else
          ptr_inc <= '1';
          next_state <= state_init_1;
        end if;
      
      when state_init_1 =>
        mx1_sel <= '0';
        next_state <= state_init_2;

      when state_init_2 =>
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        next_state <= state_init;
        
      -- fetch
      when state_fetch_01 => 
        mx1_sel <= '1';
        next_state <= state_fetch;

      when state_fetch =>
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        next_state <= state_decode;

      -- decode
      when state_decode =>
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        case DATA_RDATA is
          when X"3E" => -- >
            next_state <= state_ptr_inc;
          when X"3C" => -- <
            next_state <= state_ptr_dec;
          when X"2B" => -- +
            mx1_sel <= '0';
            next_state <= state_value_inc;
          when X"2D" => -- -
            mx1_sel <= '0';
            next_state <= state_value_dec;
          when X"5B" => -- [
            mx1_sel <= '0';
            next_state <= state_loop_start;
          when X"5D" => -- ]
            mx1_sel <= '0';
            next_state <= state_loop_end;
          when X"7E" => -- ~
            pc_inc <= '1';
            next_state <= state_break;
          when X"2E" => -- . 
            mx1_sel <= '0';
            next_state <= state_write;
          when X"2C" => -- ,
            IN_REQ <= '1';
            mx1_sel <= '0';
            next_state <= state_read;
          when X"40" => -- @
            next_state <= state_null;
          when others => -- ostatni znaky 
            pc_inc <= '1';
            next_state <= state_fetch_01;
        end case;

      -- while loop start
      when state_loop_start =>
        pc_inc <= '1';
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        next_state <= state_loop_start_1;

      when state_loop_start_1 =>
        mx1_sel <= '1';
        if DATA_RDATA = X"00" then
          if cnt_out = "0000" then
            cnt_inc <= '1';
          end if;
          next_state <= state_loop_start_2;
        else
          next_state <= state_fetch;
        end if;

      when state_loop_start_2 =>
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        next_state <= state_loop_start_3;

      when state_loop_start_3 =>
        if DATA_RDATA = X"5D" then
          cnt_dec <= '1';
          if cnt_out = "0001" then
            pc_inc <= '1';
            next_state <= state_fetch_01;
          else
            mx1_sel <= '0';
            next_state <= state_loop_start;
          end if;
        elsif DATA_RDATA = X"5B" then
          cnt_inc <= '1';
          mx1_sel <= '0';
          next_state  <= state_loop_start;
        else 
          mx1_sel <= '0';
          next_state  <= state_loop_start;
        end if;
          
      -- while loop end
      when state_loop_end =>
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        next_state <= state_loop_end_1;

      when state_loop_end_1 =>
        if DATA_RDATA /= X"00" then
          if cnt_out = "0000" then
            cnt_inc <= '1';
          end if;
          pc_dec <= '1';
          next_state <= state_loop_end_2; 
        else
          pc_inc <= '1';
          next_state <= state_fetch_01;
        end if;

      when state_loop_end_2 =>
        mx1_sel <= '1';
        next_state <= state_loop_end_2_1;

      when state_loop_end_2_1 =>
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        next_state <= state_loop_end_3;

      when state_loop_end_3 =>
        if DATA_RDATA = X"5B" then
          cnt_dec <= '1';
          if cnt_out = "0001" then
            pc_inc <= '1';
            next_state <= state_fetch_01;
          else 
            mx1_sel <= '0';
            next_state <= state_loop_end;
          end if;
        elsif DATA_RDATA = X"5D" then
          cnt_inc <= '1';
          mx1_sel <= '0';
          next_state  <= state_loop_end;
        else 
          mx1_sel <= '0';
          next_state  <= state_loop_end;
        end if;
            
      -- inkrementace
      when state_value_inc =>
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        pc_inc <= '1'; 
        next_state <= state_value_inc_1;

      when state_value_inc_1 =>
        mx2_sel <= "10";
        next_state <= state_value_inc_2;

      when state_value_inc_2 =>
        DATA_EN <= '1';
        DATA_RDWR <= '1';
        mx1_sel <= '1';
        next_state <= state_fetch;

      -- dekrementace
      when state_value_dec =>
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        pc_inc <= '1';
        next_state <= state_value_dec_1;

      when state_value_dec_1 =>
        mx2_sel <= "01";
        next_state <= state_value_dec_2;

      when state_value_dec_2 =>
        DATA_EN <= '1';
        DATA_RDWR <= '1';
        mx1_sel <= '1';
        next_state <= state_fetch;

      -- reset ?
      when state_value_rst => 
        ptr_rst <= '1';
        next_state <= state_decode; 

      -- move right
      when state_ptr_inc =>
        ptr_inc <= '1';
        pc_inc <= '1';
        next_state <= state_fetch_01;
      
      -- move left
      when state_ptr_dec =>
        ptr_dec <= '1';
        pc_inc <= '1';
        next_state <= state_fetch_01;

      -- read
      when state_read =>
        IN_REQ <= '1';
        DATA_EN <= '1';
        if IN_VLD = '1' then
          mx2_sel <= "00";
          pc_inc <= '1';
          next_state <= state_read_1;
        else 
          next_state <= state_read;
        end if;

      when state_read_1 =>
        DATA_EN <= '1';
        DATA_RDWR <= '1'; 
        mx1_sel <= '1';
        next_state <= state_fetch;

      -- write   
      when state_write =>
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        if OUT_BUSY = '1' then
          next_state <= state_write;
        else
          pc_inc <= '1';
          next_state <= state_write_1;
        end if;

      when state_write_1 =>
        OUT_WE <= '1';
        OUT_DATA <= DATA_RDATA;
        mx1_sel <= '1';
        next_state <= state_fetch;

      -- break
      when state_break =>
        mx1_sel <= '1';
        next_state <= state_break_1;

      when state_break_1 =>
        DATA_EN <= '1';
        DATA_RDWR <= '0';
        next_state <= state_break_2;
      
      when state_break_2 =>
        if DATA_RDATA = X"5D" then
          pc_inc <= '1';
          if cnt_out = "0000" then
            next_state <= state_fetch_01;
          else
            cnt_dec <= '1';
            next_state <= state_break;
          end if;
        elsif DATA_RDATA = X"5B" then
          cnt_inc <= '1';
          pc_inc <= '1';
          next_state <= state_break;
        else
          pc_inc <= '1';
          next_state <= state_break;
        end if;

      -- koncovy stav
      when state_null => 
        DONE <= '1';
        next_state <= state_null;         
      when others =>
      
    end case;
  end process;
end behavioral;
