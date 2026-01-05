import React, { createContext, useContext, useState } from 'react';

const AppContext = createContext();

export function AppProvider({children}) {
  const [count, setCount] = useState(0);
  const value = {
    count,
    increment: () => setCount(c => c + 1),
    decrement: () => setCount(c => c - 1),
  };

  return (
    <AppContext.Provider value={value}>
      {children}
    </AppContext.Provider>
  );
}

export function useApp() {
  return useContext(AppContext);
}