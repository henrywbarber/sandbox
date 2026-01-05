import { Stack } from "expo-router";
import { AppProvider } from "../context/AppContext";

export default function Layout() {
	return (
		<AppProvider>
			<Stack />
		</AppProvider>
	);
}
