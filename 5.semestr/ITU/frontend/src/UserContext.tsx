import { createContext, useContext, useState, ReactNode } from 'react';

type UserType = "user" | "worker" | "admin";

type UserContextType = {                // Typ pro data, která chceme sdílet v celé aplikaci
  login: UserType;                      // Přihlášený uživatel
  setLogin: (login: UserType) => void;  // Funkce pro změnu přihlášeného uživatele
};

// Vytvoření kontextu, který bude obsahovat naše data
const UserContext = createContext<UserContextType | undefined>(undefined);

// Funkce (hook) pro snadné získání dat z kontextu
export const useUser = () => {
  const context = useContext(UserContext);
  // Pokud se pokusíme použít tento hook mimo UserProvider, (naše aplikace je ním celá obalená, viz main.tsx)
  if (!context) {
    throw new Error("useUser must be used within a UserProvider");
  }
  return context; // Vrátí data (login a setLogin) z kontextu
};

// Komponenta, která obalí naši aplikaci a poskytne data všem ostatním komponentám
export const UserProvider = ({ children }: { children: ReactNode }) => {
  
  // Stav pro uložení přihlášeného uživatele
  const [login, setLogin] = useState<UserType>("user"); // Výchozí profil je "user"

  return (
    // Umožní všem komponentám uvnitř této obálky přístup k loginu a setLoginu
    <UserContext.Provider value={{ login, setLogin }}>
      {children}
    </UserContext.Provider>
  );
};
