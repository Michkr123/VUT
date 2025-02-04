import React from 'react';
import { createContext, useContext, useState, ReactNode } from 'react';

type UserType = "user" | "admin";

type UserContextType = {
  login: UserType;
  setLogin: (login: UserType) => void;
};

// Vytvoření kontextu, který bude obsahovat naše data
const UserContext = createContext<UserContextType | undefined>(undefined);

// Funkce (hook) pro snadné získání dat z kontextu
export const useUser = () => {
  const context = useContext(UserContext);
  // Pokud se pokusíme použít tento hook mimo UserProvider, (naše aplikace je ním celá obalená, viz toplevel app/_layout.tsx)
  if (!context) {
    throw new Error("useUser must be used within a UserProvider");
  }
  return context; // Vrátí data (login a setLogin) z kontextu
};

// Komponenta, která obalí naši aplikaci a poskytne data všem ostatním komponentám
export const UserProvider = ({ children }: { children: ReactNode }) => {
  // Stav pro uložení přihlášeného uživatele
  const [login, setLogin] = useState<UserType>("user");

  return (
    // Umožní všem komponentám uvnitř této obálky přístup k loginu a setLoginu
    <UserContext.Provider value={{ login, setLogin }}>
      {children}
    </UserContext.Provider>
  );
};
